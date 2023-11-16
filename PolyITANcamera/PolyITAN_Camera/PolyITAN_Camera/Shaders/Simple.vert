#version 300 es

precision highp float;

void main(void)
{
    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor  = vec4(gl_Color.rgb, 1);
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
