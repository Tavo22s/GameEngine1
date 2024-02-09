#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in ivec4 bone_ids;
layout (location = 6) in vec4 weights;

out VS_OUT
{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
    mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];

	vec4 boned_position = bone_transform * vec4(aPos, 1.0);

    mat4 MVP = projection * view * model;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos = vec3(model * boned_position);
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    
    gl_Position = MVP * boned_position;
}