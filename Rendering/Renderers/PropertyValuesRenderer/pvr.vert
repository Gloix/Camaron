#version 400

in vec4 VertexPosition;
in float VertexValue;
in uint VertexFlags;
uniform float minValue;
uniform float maxValue;
uniform float noValue;
uniform vec4 NoValueColor;
uniform vec4 MinValueColor;
uniform vec4 MaxValueColor;
uniform mat4 MVP;
out vec4 Color;
out uint VertexFlagsGeom;

void main()
{
    VertexFlagsGeom = VertexFlags;
    if(noValue != VertexValue){
	float rat = (VertexValue-minValue)/(maxValue-minValue);
	Color = mix(MinValueColor,MaxValueColor, rat);
    }
    else
	Color = NoValueColor;
    gl_Position = MVP*VertexPosition;
}
