#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in uint VertexFlag;
in ivec3 PPIds;
uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;
uniform int ElementDrawOption;
out vec4 VertexModelViewPosition;
out vec3 VertexNormalModelView;
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
    VertexModelViewPosition = ModelView*vec4(VertexPosition,1.0);
    gl_Position = MVP * vec4(VertexPosition,1.0);
    if(isDrawn()){
	discardThisVertex = 1.0f;
	VertexNormalModelView = NormalMatrix*VertexNormal;
    }
}
