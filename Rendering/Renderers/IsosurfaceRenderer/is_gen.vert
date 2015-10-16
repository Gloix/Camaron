#version 400

in vec3 VertexPosition;
//in uint VertexFlags;
in float VertexScalar;

struct VertexData{
    vec3 VertexPosition;
    float VertexScalar;
};

out VertexData vertexData;

void main()
{
    vertexData.VertexPosition = VertexPosition;
    vertexData.VertexScalar = VertexScalar;

}
