#version 400
uniform sampler2D TexColorsDepth;
uniform vec3 unicolor;
in vec2 FragTexCoord;
out vec4 outputColor;

void main()
{
    vec4 texColorDept = texture( TexColorsDepth, FragTexCoord );
    outputColor = vec4(unicolor*(texColorDept.r),1.0f);
}
