#version 400

in vec4 VertexPosition;
in uint VertexFlags;
uniform mat4 MVP;
uniform vec4 BaseColor;
uniform vec4 SelectedColor;
out vec4 fcolor;
void main()
{
    if((VertexFlags&1u)==1u)
	fcolor = SelectedColor;
    else
	fcolor = BaseColor;
    gl_Position = MVP*VertexPosition;
}
