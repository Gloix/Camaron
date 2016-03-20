#version 400

in vec4 ScalarValue;
out vec4 outputColor;

uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;
uniform float ScalarMin;
uniform float ScalarMax;


void main()
{
    outputColor = mix(GradientStartColor, GradientEndColor,
              (ScalarValue-ScalarMin)/(ScalarMax-ScalarMin));
}
