#version 400
in vec3 Normal;
out vec4 outputColor;

uniform float ScalarMin;
uniform float ScalarMax;
uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;
uniform mat3 NormalMatrix;
uniform float ScalarValue;


void main()
{
    vec3 lightDirection = vec3(1,1,1);
    outputColor = mix(GradientStartColor, GradientEndColor, (ScalarValue-ScalarMin)/(ScalarMax-ScalarMin));
    float diffuseFactor = abs(dot(lightDirection, normalize(NormalMatrix*Normal)));
    outputColor = vec4(outputColor.rgb*(0.8+0.2*diffuseFactor),outputColor.a);
}
