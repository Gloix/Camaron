#version 400
in vec4 GeomVertexPosition[3];
in vec4 GeomVertexMV[3];
in uint GeomVertexFlags[3];
in int GeomVertexIds[3];
in int GeomPPIds[3];
in vec3 GeomVertexNormal[3];

struct ConfigurationData{
    int drawOption;// 0 = surface, 1 = interior, 2 = complete, 3 = selected;
    int drawOptionFacing; //0 = front, 1 = back, 2 = all, 3 = closer than
    int elementsType;//polygon, vertex or polyhedron
    vec3 CameraPosition;
    float maxDistance;
    float idPercentSize;
};
uniform float CellWidth;
uniform float CellHeight;
uniform ConfigurationData config;
layout(triangles) in;
layout(triangle_strip, max_vertices = 60) out;

out vec2 TexCoord;

void drawId(int id, vec4 position,vec2 digitDimension);
int getIdToDraw();
void drawDigit(int digit,int it,vec4 position,vec2 numberDigitDim);
int getNumberOfDigits(int id);
bool discardGeometry(vec4 v,float zmv);
bool discardVertex(int i);
bool triangleIsAtSurface();
bool triangleIsSelected();
bool triangleFacingFront();
bool vertexFacingFront(int i);
bool triangleOusideViewPort();


void main()
{
    //Digits dimension
    int numberHeight = 18;
    int numberWidth = 12;
    vec2 numberDigitDim = vec2(CellWidth*numberWidth,CellHeight*numberHeight);
    numberDigitDim*=0.5f*config.idPercentSize;
    if(config.elementsType != 0){
	vec4 ps = GeomVertexPosition[0]+GeomVertexPosition[1] + GeomVertexPosition[2];
	ps/=3.0f;
	float fpoxz = (GeomVertexMV[0].z+GeomVertexMV[1].z+GeomVertexMV[2].z)/3.0;
	if(discardGeometry(ps,fpoxz))
	    return;
	//calculate position
	//draw Ids
	int id = getIdToDraw();
	if(id==-1)
	    return;
	drawId(id,ps,numberDigitDim);
	return;
    }
    for(int i = 0;i<3;i++){
	if(!discardVertex(i)){
	    int id = GeomVertexIds[i];
	    drawId(id,GeomVertexPosition[i],numberDigitDim);
	}
    }

}

void drawId(int id, vec4 position,vec2 digitDimension){
    int nDigits = getNumberOfDigits(id);
    position -= vec4(nDigits*digitDimension.x/2.0f,digitDimension.y/2.0f,0.0f,0.0f);
    nDigits = nDigits-1;
    int it = 0;
    while(it<=nDigits){
	int currentDigit = id%10;
	drawDigit(currentDigit,nDigits-it,position,digitDimension);
	it++;
	id/=10;
    }
}
int getIdToDraw(){
    if(config.elementsType == 1){
	if((GeomVertexFlags[0]&32u)!=32u)
	    return -1;
	return GeomPPIds[0];
    }
    if(config.elementsType == 2)
	return GeomPPIds[1];
    return GeomPPIds[2];
}
bool triangleIsAtSurface(){
    return (GeomVertexFlags[0]&16u) == 16u;
}
bool triangleOusideViewPort(){
    for(int i = 0;i<3;i++)
	if(GeomVertexIds[i] == -1)
	    return true;
    return false;
}
bool triangleIsSelected(){
    for(int i = 0;i<3;i++)
	if((GeomVertexFlags[i]&1u) != 1u)
	    return false;
    return true;
}
bool triangleFacingFront(){
    vec3 normalAverage = GeomVertexNormal[0]+GeomVertexNormal[1]+GeomVertexNormal[2];
    return dot(normalAverage,config.CameraPosition) < 0.0f;
}

bool vertexFacingFront(int i){
    return dot(GeomVertexNormal[i],config.CameraPosition) < 0.0f;
}

void drawDigit(int digit,int it,vec4 position,vec2 numberDigitDim){
    float f = 0.1f*digit+0.005f;
    TexCoord = vec2(f,1.0f);
    gl_Position =position+vec4(numberDigitDim.x*it,0.0f,0.0f,0.0f);
    EmitVertex();
    TexCoord = vec2(f+0.1f,1.0f);
    gl_Position =position+vec4(numberDigitDim.x*(it+1),0.0f,0.0f,0.0f);
    EmitVertex();
    TexCoord = vec2(f,0.0f);
    gl_Position =position+vec4(numberDigitDim.x*it,numberDigitDim.y,0.0f,0.0f);
    EmitVertex();
    TexCoord = vec2(f+0.1f,0.0f);
    gl_Position =position+vec4(numberDigitDim.x*(it+1),numberDigitDim.y,0.0f,0.0f);
    EmitVertex();
    EndPrimitive();
}

int getNumberOfDigits(int id){
    int n = 0;
    while(id>0){
	n++;
	id/=10;
    }
    if(n == 0)
	return 1;
    return n;
}
// drawOption { 0 = surface, 1 = interior, 2 = complete, 3 = selected; }
bool discardGeometry(vec4 position, float zmvpos){
    if(triangleOusideViewPort())
	return true;
    if(config.drawOptionFacing == 3 && config.maxDistance >= zmvpos )
	return true;
    if(config.drawOption == 0){
	if(triangleIsAtSurface()){
	    if(config.drawOptionFacing==0)
		return !triangleFacingFront();
	    if(config.drawOptionFacing==1)
		return triangleFacingFront();
	    return false;
	}
	return true;
    }
    if(config.drawOption == 2)
	return !triangleIsSelected();
    if(config.drawOption == 1)
	return triangleIsAtSurface();
    return false;//complete
}
bool discardVertex(int i){
    if(GeomVertexIds[i]==-1)
	return true;
    if(config.drawOptionFacing == 3 && config.maxDistance >= GeomVertexMV[i].z )
	return true;
    if(config.drawOption == 2)
	return (GeomVertexFlags[i]&1u)!=1u;
    else if((GeomVertexFlags[i]&4u)==4u)
	return true;
    if(config.drawOption == 0){
	if((GeomVertexFlags[i]&8u)==8u){
	    if(config.drawOptionFacing==0)
		return !vertexFacingFront(i);
	    if(config.drawOptionFacing==1)
		return vertexFacingFront(i);
	    return false;
	}
	return true;
    }
    if(config.drawOption == 1)
	return (GeomVertexFlags[i]&8u)==8u;
    return false;//complete
}
