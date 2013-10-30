#version 400
layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;
//out vec3 GNormal;
//out vec3 GPosition;
noperspective out vec3 GEdgeDistance;
noperspective out vec3 GVertexDistance;
out vec3 IgnoreEdges;
out vec4 ColorToFrag;

in vec4 Color[];
in uint VFlags[];
in vec4 GeomVertexPosition[];
in int GeomVertexInside[];
struct LineInfo {
    int Width;
    vec4 Color;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;
uniform mat4 ViewportMatrix; // Viewport matrix

uniform int ElementDrawOption;
//outside elements
uniform int drawOutsideElements;
uniform vec4 BorderColor;

bool isFlagEnabled(uint f){
    return (VFlags[0]&f)==f && (VFlags[1]&f)==f && (VFlags[2]&f)==f;
}
void main()
{
    // Transform each vertex into viewport space
    if(ElementDrawOption == 1 && !isFlagEnabled(8u))
	return;
    if(ElementDrawOption == 2 && isFlagEnabled(8u))
	return;
    if(ElementDrawOption == 3 && !isFlagEnabled(1u))
	return;
    if(drawOutsideElements==0){
	if(GeomVertexInside[0]==0&&GeomVertexInside[1]==0&&GeomVertexInside[2]==0)
	    return;
    }
    bool isBorder = GeomVertexInside[0]!=GeomVertexInside[1]||
	    GeomVertexInside[0]!=GeomVertexInside[2];
    float ha = 0.0f;
    float hb = 0.0f;
    float hc = 0.0f;
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    if(Line.isDrawn==1){
	vec3 p0 = vec3(ViewportMatrix * (GeomVertexPosition[0] /
					 GeomVertexPosition[0].w));
	vec3 p1 = vec3(ViewportMatrix * (GeomVertexPosition[1] /
					 GeomVertexPosition[1].w));
	vec3 p2 = vec3(ViewportMatrix * (GeomVertexPosition[2] /
					 GeomVertexPosition[2].w));
	// Find the altitudes (ha, hb and hc)
	p0.z = 0.0f;
	p1.z = 0.0f;
	p2.z = 0.0f;
	a = length(p1 - p2);
	b = length(p2 - p0);
	c = length(p0 - p1);
	float area = length(cross(p0-p1, p0-p2))/2.0f;

	ha = area*2/a;
	IgnoreEdges = ivec3(0.0f,0.0f,0.0f);
	if((VFlags[0]&2u)==2u)
	    IgnoreEdges.x = 1.0f;
	hb = area*2/b;
	if((VFlags[1]&2u)==2u)
	    IgnoreEdges.y = 1.0f;
	hc = area*2/c;
	if((VFlags[2]&2u)==2u)
	    IgnoreEdges.z = 1.0f;
	// Send the triangle along with the edge distances
    }
    GEdgeDistance = vec3( ha, 0, 0 );
    GVertexDistance = vec3(0,b,c);
    gl_Position = GeomVertexPosition[0];
    ColorToFrag = Color[0];
    if(isBorder&&GeomVertexInside[0]==0)
	ColorToFrag = BorderColor;
    EmitVertex();
    GEdgeDistance = vec3( 0, hb, 0 );
    GVertexDistance = vec3(a,0,c);
    gl_Position = GeomVertexPosition[1];
    ColorToFrag = Color[1];
    if(isBorder&&GeomVertexInside[1]==0)
	ColorToFrag = BorderColor;
    EmitVertex();
    GEdgeDistance = vec3( 0, 0, hc );
    GVertexDistance = vec3(a,b,0);
    gl_Position = GeomVertexPosition[2];
    ColorToFrag = Color[2];
    if(isBorder&&GeomVertexInside[2]==0)
	ColorToFrag = BorderColor;
    EmitVertex();
    EndPrimitive();
}
