#version 400

in vec4 VertexPosition;
uniform mat4 MVP;
uniform vec4 BaseColor;
out vec4 fcolor;
void main()
{
    fcolor = BaseColor;
    gl_Position = MVP*VertexPosition;
}
