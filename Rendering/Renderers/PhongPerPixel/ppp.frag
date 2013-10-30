#version 400
struct MaterialInfo;
struct LightInfo;
vec3 phongModelPreMultiplied( vec4 VertexPosition,vec3 VertexNormal,
			      LightInfo light, MaterialInfo matInfo );
uniform MaterialInfo Material;
uniform LightInfo Light;
noperspective in float discardThisVertex;
uniform int TranslucentMode;
uniform int InverseBackColor;
out vec4 FragColor;
in vec4 VertexModelViewPosition;
in vec3 VertexNormalModelView;
void main() {
    if(discardThisVertex<1.0f)
	discard;
    if(TranslucentMode == 1){
	if( gl_FrontFacing ) {
	    vec3 frontcolor = phongModelPreMultiplied( VertexModelViewPosition,
						       VertexNormalModelView,
						       Light,Material);
	    FragColor = vec4(frontcolor,length(frontcolor));
	} else {
	    vec3 backcolor = phongModelPreMultiplied( VertexModelViewPosition,
						      -VertexNormalModelView,
						      Light,Material);
	    if(InverseBackColor == 0)
		FragColor = vec4(backcolor,length(backcolor));
	    else
		FragColor = vec4(vec3(1.0,1.0,1.0)-backcolor,length(backcolor));
	}
    }
    else{
	if( gl_FrontFacing ) {
	    FragColor = vec4(phongModelPreMultiplied( VertexModelViewPosition,
						      VertexNormalModelView,
						      Light,Material), 1.0);
	} else {
	    if(InverseBackColor == 0)
		FragColor = vec4(phongModelPreMultiplied( VertexModelViewPosition,
							  -VertexNormalModelView,
							  Light,Material),1.0);
	    else
		FragColor = vec4(vec3(1.0,1.0,1.0)-phongModelPreMultiplied( VertexModelViewPosition,
									    -VertexNormalModelView,
									    Light,Material),1.0);
	}
    }
}
