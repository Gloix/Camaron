#version 400

in vec3 VertexPosition;
in vec3 VertexColor;

uniform mat4 MVP;

out vec3 fcolor;

void main()
{
    gl_Position = MVP*vec4(VertexPosition,1);
    fcolor = VertexColor;
}
