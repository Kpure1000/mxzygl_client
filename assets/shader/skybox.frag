varying vec3 fragPos;

uniform float _fov_rad;
uniform float _pitch;
uniform vec3 _sky;
uniform vec3 _ground;

void main()
{
    vec3 down   = (_pitch - _fov_rad * 0.5) * (_sky - _ground) + _ground;
    vec3 up     = (_pitch + _fov_rad * 0.5) * (_sky - _ground) + _ground;
    float pitch = (fragPos.y + 1.0) * 0.5;
    gl_FragColor = vec4( pitch * (up - down) + down, 1.0) ;
}
