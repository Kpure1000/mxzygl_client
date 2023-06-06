#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in mat4 aTrans;
layout (location = 6) in mat3 aTrans_nor;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _proj;
uniform mat3 _normal;

out vec3 fragPos;
out vec3 fragNor;

void main()
{
    vec4 pos = _model * aTrans * vec4(aPos, 1.0);
    fragPos = pos.xyz;
    fragNor = _normal * aTrans_nor * aNor;
    gl_Position = _proj * _view * pos;
}
