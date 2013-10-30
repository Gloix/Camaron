#version 400
uniform sampler2D TexColorsAlpha;
uniform sampler2D TexBackground;
uniform usampler2D TexColorsDepth;
uniform int useWeightedAverage;
in vec2 FragTexCoord;
out vec4 outputColor;
void main()
{

    uvec4 texColorDept = texture( TexColorsDepth, FragTexCoord );
    uint stencilVal = (texColorDept.r)%255u;
    vec4 texColor = texture( TexColorsAlpha, FragTexCoord );
    vec4 backgroundColor = texture( TexBackground, FragTexCoord );
    if(useWeightedAverage==1){
	float powerAlpha = 1.0f;
	if(stencilVal!=0u){
	    powerAlpha = pow(1.0f - texColor.w/stencilVal,stencilVal);
	    texColor.xyz = texColor.xyz / texColor.w;
	}
	outputColor = vec4(texColor.xyz * (1.0f - powerAlpha) +
			   backgroundColor.xyz * powerAlpha,1.0f);
    }
    else{
	outputColor = vec4(texColor.xyz +
			   backgroundColor.xyz * (1.0f-texColor.w),1.0f);

    }
}
