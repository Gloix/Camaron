#version 400

in vec3 VertexPosition;
//in uint VertexFlags;
in float VertexScalar;
in uint VertexFlags;

struct VertexData{
    vec3 VertexPosition;
    float VertexScalar;
    uint VertexFlags;
};

out VertexData vertexData;

void main()
{
    vertexData.VertexPosition = VertexPosition;
    vertexData.VertexScalar = VertexScalar;
    vertexData.VertexFlags = VertexFlags;

}
