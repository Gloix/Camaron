#version 400

in vec3 VertexPosition;
in uint VertexFlags;
in float VertexScalar;

out VertexData{
    vec3 Position;
    uint Flags;
    float ScalarValue;
} vertexData;

void main()
{
    vertexData.Position = VertexPosition;
    vertexData.ScalarValue = VertexScalar;
    vertexData.Flags = VertexFlags;

}
