#version 400

in vec3 VertexPosition;
in float VertexScalar;

uniform mat4 MVP;
out float ScalarValue;

void main()
{
    gl_Position = MVP*vec4(VertexPosition,1);
    ScalarValue = VertexScalar;

}
