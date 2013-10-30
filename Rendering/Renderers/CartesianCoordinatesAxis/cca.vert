#version 400

in vec4 VertexPosition;
uniform mat4 MVP;
uniform vec3 BaseColor;
out vec4 fcolor;
void main()
{
    fcolor = vec4(BaseColor,1.0f);
    gl_Position = MVP*VertexPosition;
}
