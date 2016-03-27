#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

uniform mat4 MVP;

out float ScalarValue;
out vec3 Normal;

void main()
{
    gl_Position = MVP * vec4(VertexPosition,1);
    Normal = VertexNormal;
}
