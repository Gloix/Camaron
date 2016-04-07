#version 400

in vec3 fcolor;
out vec4 outputColor;

void main()
{
    outputColor = vec4(fcolor,1);
}
