#version 400
in vec3 FrontColor;
in vec3 BackColor;
noperspective in float discardThisVertex;
uniform int TranslucentMode;
uniform int InverseBackColor;
out vec4 FragColor;
void main() {
    if(discardThisVertex<1.0f)
	discard;
    if(TranslucentMode == 1){
	float f = length(FrontColor);
	if( gl_FrontFacing ) {
	    FragColor = vec4(FrontColor,f);
	} else {
	    if(InverseBackColor == 0)
		FragColor = vec4(BackColor,f);
	    else
		FragColor = vec4(vec3(1.0,1.0,1.0)-BackColor,f);
	}
    }
    else{
	if( gl_FrontFacing ) {
	    FragColor = vec4(FrontColor, 1.0);
	} else {
	    if(InverseBackColor == 0)
		FragColor = vec4(BackColor,1.0);
	    else
		FragColor = vec4(vec3(1.0,1.0,1.0)-BackColor,1.0);
	}
    }
}
