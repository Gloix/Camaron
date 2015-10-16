#version 400
in float ScalarValue;
out vec4 outputColor;

uniform float ScalarMin;
uniform float ScalarMax;
uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;


void main()
{
    outputColor = mix(GradientStartColor, GradientEndColor, (ScalarValue-ScalarMin)/(ScalarMax-ScalarMin));
    //outputColor = vec4(1.0);
}
