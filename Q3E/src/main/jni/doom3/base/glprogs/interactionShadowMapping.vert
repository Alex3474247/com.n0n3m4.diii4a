#version 100
//#pragma optimize(off)

precision highp float;

//#define BLINN_PHONG
//#define _PBR
//#define _AMBIENT
//#define _POINT_LIGHT
//#define _SPOT_LIGHT
//#define _PARALLEL_LIGHT

varying vec2 var_TexDiffuse;
varying vec2 var_TexNormal;
varying vec2 var_TexSpecular;
varying vec4 var_TexLight;
varying lowp vec4 var_Color;
varying vec3 var_L;
#if defined(BLINN_PHONG) || defined(_PBR) || defined(_AMBIENT)
varying vec3 var_H;
#endif
#if (!defined(BLINN_PHONG) && !defined(_AMBIENT)) || defined(_PBR)
varying vec3 var_V;
#endif
#if defined(_PBR)
varying vec3 var_Normal;
#endif

attribute vec4 attr_TexCoord;
attribute vec3 attr_Tangent;
attribute vec3 attr_Bitangent;
attribute vec3 attr_Normal;
attribute highp vec4 attr_Vertex;
attribute lowp vec4 attr_Color;

uniform vec4 u_lightProjectionS;
uniform vec4 u_lightProjectionT;
uniform vec4 u_lightFalloff;
uniform vec4 u_lightProjectionQ;
uniform lowp float u_colorModulate; // 0 or 1/255
uniform lowp float u_colorAdd; // 0 or 1
uniform lowp vec4 u_glColor;

uniform vec4 u_lightOrigin;
uniform vec4 u_viewOrigin;

uniform vec4 u_bumpMatrixS;
uniform vec4 u_bumpMatrixT;
uniform vec4 u_diffuseMatrixS;
uniform vec4 u_diffuseMatrixT;
uniform vec4 u_specularMatrixS;
uniform vec4 u_specularMatrixT;

uniform highp mat4 u_modelViewProjectionMatrix;
#if defined(_AMBIENT)
varying mat3 var_TangentToWorldMatrix;
#endif

uniform highp mat4 u_modelMatrix;
uniform highp vec4 globalLightOrigin;

#ifdef _POINT_LIGHT
    varying highp vec3 var_LightToVertex;
    varying highp vec4 var_VertexPosition;
#else
    uniform highp mat4 shadowMVPMatrix;
    varying highp vec4 var_ShadowCoord;
#endif

void main(void)
{
    mat3 M = mat3(attr_Tangent, attr_Bitangent, attr_Normal);

    var_TexNormal.x = dot(u_bumpMatrixS, attr_TexCoord);
    var_TexNormal.y = dot(u_bumpMatrixT, attr_TexCoord);

    var_TexDiffuse.x = dot(u_diffuseMatrixS, attr_TexCoord);
    var_TexDiffuse.y = dot(u_diffuseMatrixT, attr_TexCoord);

    var_TexSpecular.x = dot(u_specularMatrixS, attr_TexCoord);
    var_TexSpecular.y = dot(u_specularMatrixT, attr_TexCoord);

    var_TexLight.x = dot(u_lightProjectionS, attr_Vertex);
    var_TexLight.y = dot(u_lightProjectionT, attr_Vertex);
    var_TexLight.z = dot(u_lightFalloff, attr_Vertex);
    var_TexLight.w = dot(u_lightProjectionQ, attr_Vertex);

#if defined(_AMBIENT)
    vec3 L = normalize(vec3(0.0, 0.5, 1.0));
    var_TangentToWorldMatrix = mat3(u_modelMatrix) * M;
#else
    vec3 L = u_lightOrigin.xyz - attr_Vertex.xyz;
#endif
    vec3 V = u_viewOrigin.xyz - attr_Vertex.xyz;
#if defined(BLINN_PHONG) || defined(_PBR) || defined(_AMBIENT)
    vec3 H = normalize(L) + normalize(V);
#endif

    var_L = L * M;
#if defined(BLINN_PHONG) || defined(_PBR) || defined(_AMBIENT)
    var_H = H * M;
#endif
#if (!defined(BLINN_PHONG) && !defined(_AMBIENT)) || defined(_PBR)
    var_V = V * M;
#endif
#if defined(_PBR)
    var_Normal = attr_Normal * M;
#endif

    var_Color = attr_Color * u_colorModulate + u_colorAdd;

#ifdef _POINT_LIGHT
    highp vec4 posInLight = u_modelMatrix * attr_Vertex;
    var_LightToVertex = posInLight.xyz - globalLightOrigin.xyz;
    var_VertexPosition = attr_Vertex;
#else
    var_ShadowCoord = attr_Vertex * shadowMVPMatrix;
#endif
    gl_Position = u_modelViewProjectionMatrix * attr_Vertex;
}
