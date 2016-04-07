#version 400
struct MaterialInfo1D {
    float Ka; // Ambient reflectivity
    float Kd; // Diffuse reflectivity
    float Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
};

struct LightInfo1D {
    vec4 Position; // Light position in eye coords.
    float La; // Ambient light intensity
    float Ld; // Diffuse light intensity
    float Ls; // Specular light intensity
};
float phongModel1D( vec3 VertexPosition,vec3 VertexNormal,mat3 NormalMatrix ,
		 mat4 ModelViewMatrix,
		 LightInfo1D light, MaterialInfo1D matInfo );

in vec3 VertexPosition;
in vec3 VertexNormal;
in uint VertexFlag;
uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;
uniform MaterialInfo1D Material;
uniform LightInfo1D Light;
uniform int ElementDrawOption;
uniform vec4 baseModelColor;
uniform vec4 selectedModelColor;
out vec4 VertexColor;
noperspective out float discardThisVertex;
bool isDrawn(){
    if(ElementDrawOption==3)
	return (VertexFlag&1u)==1u;
    if(ElementDrawOption==4)
	return (VertexFlag&1u)!=1u;
    return true;
}

void main()
{
    discardThisVertex = 0.0f;
    if(isDrawn()){
	discardThisVertex = 1.0f;
	float VertexColorIntensity = phongModel1D( VertexPosition, VertexNormal, NormalMatrix,
				 ModelView,Light,Material);
	float f = VertexColorIntensity;
	if((VertexFlag&1u)==1u)
	    VertexColor = vec4(selectedModelColor.xyz,selectedModelColor.w*f);
	else
	    VertexColor = vec4(baseModelColor.xyz,baseModelColor.w*f);
	gl_Position = MVP * vec4(VertexPosition,1.0);
    }
}

