varying vec3 fragPos;

uniform float _pitch;
uniform vec3 _sky;
uniform vec3 _ground;

#define PI 3.1415926

void main()
{
    float pitch = (fragPos.y + 1.0) * 0.5;
    gl_FragColor = vec4( _sky * pitch + _ground * (1.0 - pitch), 1.0);
}
