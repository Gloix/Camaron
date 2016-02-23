#version 400

in vec3 VertexPosition;
in uint VertexFlags;
in float VertexScalar;

uniform mat4 MVP;

out VertexData vdata;

void main()
{
    gl_Position = MVP*VertexPosition;
    scalarValue = VertexScalar;
}
