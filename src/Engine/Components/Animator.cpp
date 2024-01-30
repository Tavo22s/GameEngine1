#include "Animator.hpp"
#include "../Time.hpp"

Animator::Animator(GameObject *gO, Scene *sn):Component("animator", gO, sn)
{
    aniTime = .0f;
    m_pAnimation = nullptr;
    m_pRoot = nullptr;
    m_pTransitionAnimation = nullptr;
}

Animator::~Animator()
{
}

void Animator::Init()
{
    m_pRoot = gameObject->childrens[0];
}

void Animator::Update()
{
    if(m_pAnimation && m_pRoot)
    {
        m_pSkinnedMeshRender->transforms.clear();
        
        float totalAnimationTime = m_pAnimation->GetDuration() / m_pAnimation->GetTicksPerSecond();

        float transitionFactor = glm::clamp(aniTime / totalAnimationTime, .0f, 1.f);

        boneTransform(aniTime, m_pSkinnedMeshRender->transforms);

        if(m_pTransitionAnimation)
        {
            std::vector<glm::mat4> transitionTransform;
            boneTransform(aniTime, transitionTransform);

            for(std::size_t i=0; i < m_pSkinnedMeshRender->transforms.size(); ++i)
            {
                for (int row = 0; row < 4; ++row)
                {
                    for (int col = 0; col < 4; ++col)
                    {
                        m_pSkinnedMeshRender->transforms[i][row][col] =
                            m_pSkinnedMeshRender->transforms[i][row][col] * (1.0f - transitionFactor) +
                            transitionTransform[i][row][col] * transitionFactor;
                    }
                }
            }

            aniTime += Time.deltaTime * m_pTransitionAnimation->speed;
            if(aniTime >= totalAnimationTime + m_fTransitionDuration)
            {
                m_pAnimation = m_pTransitionAnimation;
                m_pTransitionAnimation = nullptr;
                aniTime = .0f;
            }
        }
        else
        {
            aniTime += Time.deltaTime * m_pAnimation->speed;
        } 
    }
}

void Animator::SetAnimation(Animation *pAnimation)
{
    aniTime = .0f;
    m_pAnimation = pAnimation;
}

void Animator::SetTransitionAnimation(Animation *pTransitionAnimation, float transitionDuration)
{
    m_pTransitionAnimation = pTransitionAnimation;
    m_fTransitionDuration = transitionDuration;
}

void Animator::boneTransform(double time_in_sec, std::vector<glm::mat4> &transforms)
{
    glm::mat4 identity_mat = glm::mat4(1.f);
    double time_in_ticks = time_in_sec * m_pAnimation->GetTicksPerSecond();
    float animation_time = fmod(time_in_ticks, m_pAnimation->GetDuration());
    
    readNodeHiearchy(animation_time, m_pRoot, identity_mat);
    transforms.resize(m_pMesh->m_num_bones);
    for(unsigned int i=0; i<m_pMesh->m_num_bones; i++)
        transforms[i] = m_pMesh->m_bone_matrices[i].final_world_transform;
}

void Animator::readNodeHiearchy(float p_animation_time, const GameObject *pNode, glm::mat4 parent_transform)
{
    std::string node_name = pNode->name;
    glm::mat4 node_transform = pNode->transform->GetOpenGLMat();
    std::vector<TransformKey*> node_anim = m_pAnimation->GetRootKeys()  [node_name];
    if(node_anim.size() != 0)
        node_transform = calcInterpolatedTransform(p_animation_time, node_anim);
    glm::mat4 global_transform = parent_transform * node_transform;
    if(m_pMesh->m_bone_mapping.find(node_name) != m_pMesh->m_bone_mapping.end())
    {
        unsigned int bone_index = m_pMesh->m_bone_mapping[node_name];
        m_pMesh->m_bone_matrices[bone_index].final_world_transform = m_pMesh->m_global_inverse_transform * global_transform * m_pMesh->m_bone_matrices[bone_index].offset_matrix;
    }
    for(unsigned int i=0; i < pNode->childrens.size(); i++)
        readNodeHiearchy(p_animation_time, pNode->childrens[i], global_transform);
}

glm::mat4 Animator::calcInterpolatedTransform(float p_animation_time, std::vector<TransformKey*> v_node_anim)
{
    if(v_node_anim.size() == 1)
    {
        glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), v_node_anim[0]->scale);
        glm::mat4 rot_mat = glm::mat4_cast(v_node_anim[0]->rotation);
        glm::mat4 pos_mat = glm::translate(glm::mat4(1.f), v_node_anim[0]->position);
        return pos_mat * rot_mat * scale_mat;
    }
    unsigned int transform_index = findTransformKey(p_animation_time, v_node_anim);
    unsigned int next_transform_index = transform_index + 1;
    assert(next_transform_index < v_node_anim.size());

    float delta_time = (float) (v_node_anim[next_transform_index]->m_dTime - v_node_anim[transform_index]->m_dTime);
    
    float factor = (p_animation_time - (float) v_node_anim[transform_index]->m_dTime) / delta_time;
    assert(factor >= .0f && factor <= 1.f);

    glm::vec3 scaling_vec = InterpolateVec3(v_node_anim[transform_index]->scale, v_node_anim[next_transform_index]->scale, factor);
    glm::quat rotation_quat = nlerp(v_node_anim[transform_index]->rotation, v_node_anim[next_transform_index]->rotation, factor);
    glm::vec3 position_vec = InterpolateVec3(v_node_anim[transform_index]->position, v_node_anim[next_transform_index]->position, factor);
    
    return glm::translate(glm::mat4(1.f), position_vec) * glm::mat4_cast(rotation_quat) * glm::scale(glm::mat4(1.f), scaling_vec);
}

unsigned int Animator::findTransformKey(float p_animation_time, std::vector<TransformKey*> v_node_anim)
{
    for(unsigned int i=0; i<v_node_anim.size(); i++)
    {
        if(p_animation_time < (float) v_node_anim[i+1]->m_dTime)
            return i;
    }
    assert(0);
    return 0;
}

glm::vec3 Animator::InterpolateVec3(glm::vec3 start, glm::vec3 end, float factor)
{
    glm::vec3 delta = end - start;
    return start + factor * delta;
}

glm::quat Animator::nlerp(glm::quat start, glm::quat end, float factor)
{
    start = glm::normalize(start);
    end = glm::normalize(end);

    glm::quat result;
    float dot_product = glm::dot(start, end);
    float one_minus_blend = 1.f - factor;

    if(dot_product < .0f)
    {
        result.x = start.x * one_minus_blend + factor * -end.x;
        result.y = start.y * one_minus_blend + factor * -end.y;
        result.z = start.z * one_minus_blend + factor * -end.z;
        result.w = start.w * one_minus_blend + factor * -end.w;
    }
    else
    {
        result.x = start.x * one_minus_blend + factor * end.x;
        result.y = start.y * one_minus_blend + factor * end.y;
        result.z = start.z * one_minus_blend + factor * end.z;
        result.w = start.w * one_minus_blend + factor * end.w;
    }
    return glm::normalize(result);
}