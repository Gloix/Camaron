#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;
noperspective out vec3 GEdgeDistance;
out vec4 ColorToFrag;
flat out uvec3 VertexFlagsFrag;
in vec4 Color[];
in uint VertexFlagsGeom[];
uniform mat4 ViewportMatrix; // Viewport matrix
uniform int DrawOnlySelectedElements;

struct LineInfo {
    int Width;
    vec4 Color;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;

void main()
{
    if(DrawOnlySelectedElements==1 && (
	    (VertexFlagsGeom[0]&1u)!=1u ||
	    (VertexFlagsGeom[1]&1u)!=1u ||
	    (VertexFlagsGeom[2]&1u)!=1u))
	return;//dont draw unselected polygon
    // Transform each vertex into viewport space
    float ha = 0.0f;
    float hb = 0.0f;
    float hc = 0.0f;
    VertexFlagsFrag = uvec3(0u,0u,0u);
    if(Line.isDrawn==1){
	vec3 p0 = vec3(ViewportMatrix * (gl_in[0].gl_Position /
					 gl_in[0].gl_Position.w));
	vec3 p1 = vec3(ViewportMatrix * (gl_in[1].gl_Position /
					 gl_in[1].gl_Position.w));
	vec3 p2 = vec3(ViewportMatrix * (gl_in[2].gl_Position /
					 gl_in[2].gl_Position.w));
	// Find the altitudes (ha, hb and hc)
	p0.z = 0.0f;
	p1.z = 0.0f;
	p2.z = 0.0f;
	float a = length(p1 - p2);
	float b = length(p2 - p0);
	float c = length(p0 - p1);
	float area = length(cross(p0-p1, p0-p2))/2.0f;
	VertexFlagsFrag = uvec3(VertexFlagsGeom[0],VertexFlagsGeom[1],VertexFlagsGeom[2]);

	ha = area*2/a;
	hb = area*2/b;
	hc = area*2/c;
	// Send the triangle along with the edge distances
    }
    GEdgeDistance = vec3( ha, 0, 0 );
    //GNormal = VNormal[0];
    //  GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    ColorToFrag = Color[0];
    EmitVertex();
    GEdgeDistance = vec3( 0, hb, 0 );
    //GNormal = VNormal[1];
    // GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
    ColorToFrag = Color[1];
    EmitVertex();
    GEdgeDistance = vec3( 0, 0, hc );
    //GNormal = VNormal[2];
    //GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    ColorToFrag = Color[2];
    EmitVertex();
    EndPrimitive();
}

