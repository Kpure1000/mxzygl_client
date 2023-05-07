#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _proj;

out vec3 fragPos;
out vec3 fragNor;

void main()
{
    fragPos = vec3(_model * vec4(aPos, 1.0));
    fragNor = mat3(transpose(inverse(_model))) * aNor;
    gl_Position = _proj * _view * _model * vec4(aPos, 1.0);
}
