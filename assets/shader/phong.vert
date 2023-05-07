attribute vec3 aPos;
attribute vec3 aNor;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _proj;
uniform mat4 _normal;

varying vec3 fragPos;
varying vec3 fragNor;

void main()
{
    fragPos = vec3(_model * vec4(aPos, 1.0));
    fragNor = mat3(_normal[0].xyz, _normal[1].xyz, _normal[2].xyz) * aNor;
    gl_Position = _proj * _view * _model * vec4(aPos, 1.0);
}
