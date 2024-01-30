#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.hpp"
#include "../Render/Animation.hpp"
#include "../Render/Mesh.hpp"
#include "SkinnedMeshRender.hpp"

class Animator:public Component
{
    public:
        Animator(GameObject *, Scene *);
        ~Animator();

        void Init();
        void Update();
    
        GameObject *m_pRoot;
        inline void SetMesh(Mesh *mesh) {   m_pMesh = mesh;  }
        inline void SetSkinnedMeshRender(SkinnedMeshRender *ps) {   m_pSkinnedMeshRender = ps;  }
        void SetTransitionAnimation(Animation *pTransitionAnimation, float transitionDuration);
        void SetAnimation(Animation *pAnimation);
        
    private:
        void boneTransform(double time_in_sec, std::vector<glm::mat4> &transforms);
        void readNodeHiearchy(float p_animation_time, const GameObject *pNode, glm::mat4 parent_transform);
        glm::mat4 calcInterpolatedTransform(float p_animation_time, std::vector<TransformKey*> v_node_anim);
        unsigned int findTransformKey(float p_animation_time, std::vector<TransformKey*> v_node_anim);
        glm::vec3 InterpolateVec3(glm::vec3 start, glm::vec3 end, float factor);
        glm::quat nlerp(glm::quat start, glm::quat end, float factor);

        Animation *m_pAnimation;
        Animation *m_pTransitionAnimation;
        Mesh *m_pMesh;
        SkinnedMeshRender *m_pSkinnedMeshRender;
        float aniTime;
        float m_fTransitionDuration;
};

#endif