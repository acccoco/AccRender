#version 330 core
layout (location = 0) in vec3 aPos;

#ifdef UNIFORM_BLOCK
layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};
#else
uniform mat4 view;
uniform mat4 projection;
#endif

out vec3 TexVec;


void main() {
    mat4 rot_view = mat4(mat3(view));
    vec4 pos = projection * rot_view * vec4(aPos, 1.0);
    // 让天空盒的深度始终最大
    gl_Position = pos.xyww;

    TexVec = aPos;
}