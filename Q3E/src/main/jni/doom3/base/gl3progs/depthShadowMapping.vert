#version 300 es
//#pragma optimize(off)

precision highp float;

in highp vec4 attr_Vertex;

uniform highp mat4 u_modelViewProjectionMatrix;
uniform highp vec4 u_lightOrigin;

// uniform lowp float u_uniformParm0; // .w 1.0 or 0.0

void main(void)
{
    highp vec4 pos;
    //pos = attr_Vertex.w * u_lightOrigin + attr_Vertex - u_lightOrigin;
    //float w = max(attr_Vertex.w, u_uniformParm0);
    //pos = w * u_lightOrigin + vec4(attr_Vertex.xyz, w) - u_lightOrigin;
    //pos = attr_Vertex - u_lightOrigin; pos = (pos.wwww * u_lightOrigin) + pos;
    pos = vec4(attr_Vertex.xyz, 1.0);
    gl_Position = pos * u_modelViewProjectionMatrix;
}
