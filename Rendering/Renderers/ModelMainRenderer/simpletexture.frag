#version 400
uniform sampler2D textureObj;
uniform int finalStage;
uniform vec4 backgroundColor;
in vec2 FragTexCoord;
out vec4 outputColor;

void main()
{

    vec4 texColorDept = texture( textureObj, FragTexCoord );
    if(finalStage==1)
	 outputColor.xyz = backgroundColor.xyz*texColorDept.a+texColorDept.xyz;
    else
	outputColor = texColorDept;
}
