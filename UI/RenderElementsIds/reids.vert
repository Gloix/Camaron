#version 400

in vec4 VertexPosition;
in uint VertexFlags;
in int VertexIds;
in int PPIds;
in vec3 VertexNormal;

uniform mat4 VP;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NormalMatrix;
uniform int orthoProjection;

uniform vec2 viewPortOriginalSize;
out vec4 GeomVertexPosition;
out vec4 GeomVertexMV;
out uint GeomVertexFlags;
out int GeomVertexIds;
out int GeomPPIds;
out vec3 GeomVertexNormal;

void main()
{

    GeomVertexPosition = MVP*VertexPosition;
    GeomVertexMV = MV*VertexPosition;
    GeomVertexPosition/=GeomVertexPosition.w;
    vec4 ViewPortPos = VP*GeomVertexPosition;
    if(ViewPortPos.x <= 0 || ViewPortPos.y <= 0 ||
	    ViewPortPos.x >= viewPortOriginalSize.x || ViewPortPos.y >= viewPortOriginalSize.y){
	GeomVertexIds = -1;//invalid id
	GeomPPIds = -1;
	GeomVertexFlags = VertexFlags;
	return;
    }else{
	GeomVertexFlags = VertexFlags;
	GeomVertexIds = VertexIds;
	GeomVertexNormal = NormalMatrix*VertexNormal;
	GeomPPIds = PPIds;
    }
}
