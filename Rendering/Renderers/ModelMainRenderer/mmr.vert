#version 400

in vec4 VertexPosition;
in uint flags;
uniform vec4 ModelBaseColor;
uniform vec4 SelectedElementColor;
uniform mat4 MVP;
out vec4 Color;
out uint VFlags;
out vec4 VClipPosition;
void main()
{
    Color = ModelBaseColor;
    if((flags&1u)==1u)
	Color = SelectedElementColor;
    VClipPosition = MVP*VertexPosition;
    VFlags = flags;
}
