#version 400
uniform vec4 textColor;
uniform vec4 textFillColor;
uniform sampler2D Tex1;
in vec2 TexCoord;
out vec4 outputColor;

void main()
{
    //outputColor = outputColor|fcolor;
    vec4 texColor = texture( Tex1, TexCoord );
    //outputColor = textColor;
   // outputColor.w = 1.0f-texColor.w;
    if(textFillColor.w!=0.0f){
	outputColor = mix(textColor,textFillColor,texColor.w);
    }else{
	outputColor = textColor;
	outputColor.w = 1.0f-texColor.w;
    }
}
