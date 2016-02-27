#version 400
in VertexData{
    int Id;
    vec2 Position;
    uint Flag;
    vec3 Normal;
} vdata[3];
struct ConfigurationData{
    int onlySurface, onlyInterior, onlyFront, onlyBack,closestToCamera;
    int elementsType;//polygon, vertex or polyhedron
    int selectionType;
    int rectSelection;
    float pixelTolerance;
    vec2 start,end;
    vec3 CameraPosition;
};
uniform float CellWidth;
uniform int BufferWidthHeight;
uniform ConfigurationData config;
layout(triangles) in;
layout(points, max_vertices = 3) out;

flat out uint fcolor;

void getPosition(int id,out vec4 position);
bool selectVertex(vec2 position, uint flag);
bool selectPolygon();
uint getMask(int id);
void main()
{
    if(config.elementsType != 0){
	if(selectPolygon()){
	    getPosition(vdata[0].Id,gl_Position);
	   // fcolor = 255u;
	    fcolor = getMask(vdata[0].Id);
	    EmitVertex();
	    EndPrimitive();
	}
    }

}
uint getMask(int id){
    uint div = uint(id%32);
    uint mask = 1u;
    mask = mask<<div;
    return mask;
    //return 255u;
}

void getPosition(int id,out vec4 position){
    int vertical = (id/32)/BufferWidthHeight;
    int horizontal = (id/32)%BufferWidthHeight;
    position = vec4(-1.0f+horizontal*CellWidth,-1.0f+vertical*CellWidth,0.0f,1.0f);
}
//VERTEX = 0;
//POLYGON = 1;
//POLYHEDRON = 2;


bool selectVertex(vec2 position, uint flags){
    //se podria chequear con la normal si la cara ta apuntando pa atras
    if(config.onlySurface == 1 && config.elementsType == 0 && (flags&8u)==0u)
	return false;
    if(config.rectSelection==1){
        if(position.x>=config.start.x &&
                position.y>=config.start.y &&
                position.x<=config.end.x &&
                position.y<=config.end.y)
	    return true;
	return false;
    }
    if(distance(position,config.start)<(config.pixelTolerance))
	return true;
    return false;
}

bool triangleIsAtSurface(){
    return (vdata[0].Flag&16u) == 16u;
}
bool pointInsidePolygon(){
    bool left = true;
    bool right = true;
    for(int i = 0; i < 3;i++){
	    //check
	    float calc = (config.start.y - vdata[i].Position.y)*(vdata[(i+1)%3].Position.x - vdata[i].Position.x) -
		    (config.start.x - vdata[i].Position.x)*(vdata[(i+1)%3].Position.y - vdata[i].Position.y);
	    left = left && calc > 0;
	    right = right && calc < 0;
    }
    return left || right;
}

bool selectPolygon(){
    if(config.onlySurface == 1 && config.elementsType == 1){
	if(!triangleIsAtSurface())
	    return false;
	vec3 nCal = vdata[0].Normal+vdata[1].Normal+vdata[2].Normal;
	if(config.onlyFront==1 && (dot(nCal,config.CameraPosition) > 0.0f))
	    return false;
	if(config.onlyBack==1 && (dot(nCal,config.CameraPosition) < 0.0f))
	    return false;
    }
    else if(config.onlyInterior == 1 &&
	    config.elementsType == 1 && triangleIsAtSurface() )
	return false;

    if(config.rectSelection == 1){
	for(int i = 0; i<3;i++){
            if(!selectVertex(vdata[i].Position, vdata[i].Flag))
		return false;//there is one unselected vertex
	}
	return true;
    }
    return pointInsidePolygon();
}
