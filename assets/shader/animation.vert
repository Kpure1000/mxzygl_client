attribute vec3 aPos;
attribute vec3 aNor;
//attribute mat4 aTrans;
//attribute mat3 aTrans_nor;
attribute vec4 aTrans0;
attribute vec4 aTrans1;
attribute vec4 aTrans2;
attribute vec4 aTrans3;
attribute vec3 aTrans_nor0;
attribute vec3 aTrans_nor1;
attribute vec3 aTrans_nor2;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _proj;
uniform mat3 _normal;

varying vec3 fragPos;
varying vec3 fragNor;

void main()
{
    vec4 pos = _model * mat4(aTrans0,aTrans1,aTrans2,aTrans3) * vec4(aPos, 1.0);
    fragPos = pos.xyz;
    fragNor = _normal * mat3(aTrans_nor0,aTrans_nor1,aTrans_nor2) * aNor;
    gl_Position = _proj * _view * pos;
}
