#version 400
// *** Insert appropriate uniforms for the Phong model ***
// The mesh line settings
struct LineInfo {
    int Width;
    vec4 Color;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;
//in vec3 GPosition;
//in vec3 GNormal;
noperspective in vec3 GEdgeDistance;
noperspective in vec3 GVertexDistance;
in vec3 IgnoreEdges;
in vec4 ColorToFrag;
out vec4 FragColor;

void main() {
    if(Line.isDrawn==1){
	float d = 10.0f;//more than line width
	if(IgnoreEdges[0]==0.0f)
	    d = min(d,GEdgeDistance.x);
	if(IgnoreEdges[1]==0.0f)
	    d = min(d,GEdgeDistance.y);
	if(IgnoreEdges[2]==0.0f)
	    d = min(d,GEdgeDistance.z);
	float mixVal = smoothstep(Line.Width -1, Line.Width +1, d);
	FragColor = mix( Line.Color,ColorToFrag , mixVal );
    }
    else
	FragColor = ColorToFrag;
}
