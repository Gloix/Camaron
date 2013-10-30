#version 400

in vec4 VertexPosition;
in uint VertexFlags;
in int VertexIds;
in vec3 VertexNormal;

uniform mat4 MVPVP;
uniform mat4 VP;
uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform int orthoProjection;
struct VertexData{
    int Id;
    vec2 Position;
    uint Flag;
    vec3 Normal;
};

uniform vec2 viewPortOriginalSize;
out VertexData vdata;

void main()
{
    if(orthoProjection==1)
	vdata.Position = vec2(MVPVP*VertexPosition);
    else{
	vec4 mvpCoord = MVP*VertexPosition;
	mvpCoord /= mvpCoord.w;
	vdata.Position = vec2(VP*mvpCoord);
    }
    vdata.Position.y = viewPortOriginalSize.y-vdata.Position.y;
    vdata.Flag = VertexFlags;
    vdata.Id = VertexIds;
    vdata.Normal = NormalMatrix*VertexNormal;
}
