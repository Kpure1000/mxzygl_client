#version 330 core

layout (location = 0) in vec3 aPos;

//varying vec3 fragPos;
out vec3 fragPos;

void main()
{
    fragPos = aPos;
    gl_Position = vec4(aPos, 1.0);
}
