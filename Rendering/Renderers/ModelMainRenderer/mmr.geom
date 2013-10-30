#version 400
layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;
in vec4 Color[];
in uint VFlags[];
in vec4 VClipPosition[];
noperspective out vec3 GEdgeDistance;
out vec3 IgnoreEdges;
out vec4 ColorToFrag;
uniform mat4 ViewportMatrix;
uniform int ElementDrawOption;
struct LineInfo {
    int Width;
    vec4 Color;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;
bool primitiveIsDrawn();
bool isFlagEnabled(uint f);
vec3 getVerticesDistanceFromOppositeEdge(vec4 Vertex1ClipPositions,
					 vec4 Vertex2ClipPositions,
					 vec4 Vertex3ClipPositions,
					 uint Vertex1Flag,
					 uint Vertex2Flag,
					 uint Vertex3Flag,
					 mat4 viewPortMatrix);
vec3 getWireFrameIgnoredEdges(uint Vertex1Flag,
			      uint Vertex2Flag,
			      uint Vertex3Flag);

void main()
{
    // Transform each vertex into viewport space
    if(primitiveIsDrawn()){
	vec3 heights = vec3(0.0,0.0,0.0);

	if(Line.isDrawn==1){
	    heights = getVerticesDistanceFromOppositeEdge(VClipPosition[0],
							  VClipPosition[1],
							  VClipPosition[2],
							  VFlags[0],
							  VFlags[1],
							  VFlags[2],
							  ViewportMatrix);
	    IgnoreEdges = getWireFrameIgnoredEdges(VFlags[0],
						   VFlags[1],
						   VFlags[2]);
	}

	GEdgeDistance = vec3( heights.x, 0, 0 );
	gl_Position = VClipPosition[0];
	ColorToFrag = Color[0];
	EmitVertex();
	GEdgeDistance = vec3( 0, heights.y, 0 );
	gl_Position = VClipPosition[1];
	ColorToFrag = Color[1];
	EmitVertex();
	GEdgeDistance = vec3( 0, 0, heights.z );
	gl_Position = VClipPosition[2];
	ColorToFrag = Color[2];
	EmitVertex();
	EndPrimitive();
    }
}

bool primitiveIsDrawn(){
   // if(isFlagEnabled(8u))
	//return false;//quitar esto despues
    if(ElementDrawOption == 1 && !isFlagEnabled(8u))
	return false;
    else if(ElementDrawOption == 2 && isFlagEnabled(8u))
	return false;
    else if(ElementDrawOption == 3 && !isFlagEnabled(1u))
	return false;
    else if(ElementDrawOption == 4 && isFlagEnabled(1u))
	return false;
    else
	return true;
}

bool isFlagEnabled(uint f){
    return (VFlags[0]&f)==f && (VFlags[1]&f)==f && (VFlags[2]&f)==f;
}

vec3 getVerticesDistanceFromOppositeEdge(vec4 Vertex1ClipPositions,
					 vec4 Vertex2ClipPositions,
					 vec4 Vertex3ClipPositions,
					 uint Vertex1Flag,
					 uint Vertex2Flag,
					 uint Vertex3Flag,
					 mat4 viewPortMatrix){
    vec3 p0 = vec3(viewPortMatrix * (Vertex1ClipPositions /
				     Vertex1ClipPositions.w));
    vec3 p1 = vec3(viewPortMatrix * (Vertex2ClipPositions /
				     Vertex2ClipPositions.w));
    vec3 p2 = vec3(viewPortMatrix * (Vertex3ClipPositions /
				     Vertex3ClipPositions.w));
    // Find the altitudes (ha, hb and hc)
    p0.z = 0.0f;
    p1.z = 0.0f;
    p2.z = 0.0f;
    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p0 - p1);
    float area = length(cross(p0-p1, p0-p2))/2.0f;
    return vec3(area*2/a,area*2/b,area*2/c);

}
vec3 getWireFrameIgnoredEdges(uint Vertex1Flag,
			      uint Vertex2Flag,
			      uint Vertex3Flag){
    vec3 IgnoreEdges = vec3(0.0f,0.0f,0.0f);
    if((VFlags[0]&2u)==2u)
	IgnoreEdges.x = 1.0f;
    if((VFlags[1]&2u)==2u)
	IgnoreEdges.y = 1.0f;
    if((VFlags[2]&2u)==2u)
	IgnoreEdges.z = 1.0f;
    return IgnoreEdges;

}
