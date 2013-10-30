#version 400

in vec4 VertexPosition;
in uint VertexFlags;
in vec3 VertexNormal;

uniform int WireFrameOption;
uniform mat4 MVP;
uniform float NormalLength;
struct VertexData{
    vec4 VertexTailPosition;
    vec4 VertexHeadPosition;
    uint VertexFlags;
};

out VertexData vdata;

void main()
{
    vdata.VertexTailPosition = MVP*VertexPosition;
    vdata.VertexFlags = VertexFlags;
    if((VertexFlags&8u)==8u){
	vec4 normal = vec4(NormalLength*VertexNormal,0.0f);
	vdata.VertexHeadPosition = MVP*(VertexPosition+normal);
    }

}
