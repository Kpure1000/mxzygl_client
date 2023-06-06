#version 330 core

in vec3 fragPos;
out vec4 fragCol;

uniform float _fov_rad;
uniform float _pitch;
uniform vec3 _sky;
uniform vec3 _ground;

void main()
{
    vec3 down   = (_pitch - _fov_rad * 0.5) * (_sky - _ground) + _ground;
    vec3 up     = (_pitch + _fov_rad * 0.5) * (_sky - _ground) + _ground;
    float pitch = (fragPos.y + 1.0) * 0.5;
    fragCol = vec4( pitch * (up - down) + down, 1.0) ;
}
