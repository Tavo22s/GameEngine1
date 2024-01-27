#include "Animation.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Animation::Animation()
{
    speed = 1.f;
    duration = .0f;
    name = "default";
    ticks_per_second = 25.f;
}

Animation::~Animation()
{
}

std::vector<Animation *>LoadAnimation(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout<<"Error Assimp Animation: "<<import.GetErrorString()<<std::endl;
        return {};
    }
    std::vector<Animation *> vAnimations;
    unsigned int nAnim = scene->mNumAnimations;
    for(unsigned int i=0; i<nAnim; i++)
    {
        aiAnimation *anim = scene->mAnimations[i];
        Animation *pAnimation = new Animation();
        if(scene->mAnimations[0]->mTicksPerSecond != .0f)
            pAnimation->ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
        else
            pAnimation->ticks_per_second = 25.f;
        pAnimation->name = anim->mName.C_Str();
        std::cout<<"Load Animation: "<<pAnimation->name<<std::endl;
        pAnimation->duration = anim->mDuration;
        std::cout<<pAnimation->duration<<std::endl;

        for(unsigned int i = 0; i<anim->mNumChannels; i++)
        {
            std::string sChannelsName = anim->mChannels[i]->mNodeName.C_Str();
            std::vector<TransformKey*> mTransformKeys;
            for(unsigned int j = 0; j < anim->mChannels[i]->mNumPositionKeys; j++)
            {
                TransformKey *pTransformKey = new TransformKey();
                pTransformKey->m_dTime = anim->mChannels[i]->mPositionKeys[j].mTime;
                pTransformKey->position = glm::vec3(anim->mChannels[i]->mPositionKeys[j].mValue.x,
                    anim->mChannels[i]->mPositionKeys[j].mValue.y,
                    anim->mChannels[i]->mPositionKeys[j].mValue.z);
                pTransformKey->rotation = glm::quat(anim->mChannels[i]->mRotationKeys[j].mValue.w,
                    anim->mChannels[i]->mRotationKeys[j].mValue.x,
                    anim->mChannels[i]->mRotationKeys[j].mValue.y,
                    anim->mChannels[i]->mRotationKeys[j].mValue.z);
                pTransformKey->scale = glm::vec3(anim->mChannels[i]->mScalingKeys[j].mValue.x,
                    anim->mChannels[i]->mScalingKeys[j].mValue.y,
                    anim->mChannels[i]->mScalingKeys[j].mValue.z);
                mTransformKeys.push_back(pTransformKey);
            }
            pAnimation->RootKeys.insert(std::pair<std::string, std::vector<TransformKey*>> (sChannelsName, mTransformKeys));
        }
        vAnimations.push_back(pAnimation);
    }
    return vAnimations;
}