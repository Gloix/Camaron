#version 400
struct MaterialInfo;
struct LightInfo;
vec3 phongModel( vec3 VertexPosition,vec3 VertexNormal,mat3 NormalMatrix ,
		 mat4 ModelViewMatrix,
		 LightInfo light, MaterialInfo matInfo );

in vec3 VertexPosition;
in vec3 VertexNormal;
in uint VertexFlag;
uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;
uniform MaterialInfo Material;
uniform LightInfo Light;
uniform int ElementDrawOption;
out vec3 FrontColor;
out vec3 BackColor;
noperspective out float discardThisVertex;
bool isDrawn(){
    if((VertexFlag&8u)!=8u)
	return false;//quitar esto despues
    if(ElementDrawOption==3)
	return (VertexFlag&1u)==1u;
    if(ElementDrawOption==4)
	return (VertexFlag&1u)!=1u;
    return true;
}

void main()
{
    discardThisVertex = 0.0f;
    if(isDrawn())
	discardThisVertex = 1.0f;
    FrontColor = phongModel( VertexPosition, VertexNormal, NormalMatrix,
			     ModelView,Light,Material);
    BackColor = phongModel( VertexPosition, -VertexNormal, NormalMatrix,
			    ModelView,Light,Material);
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
