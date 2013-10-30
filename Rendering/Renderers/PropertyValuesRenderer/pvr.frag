#version 400
// *** Insert appropriate uniforms for the Phong model ***
// The mesh line settings
struct LineInfo {
    int Width;
    vec4 Color;
    int SelectedWidth;
    vec4 SelectedColor;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;
noperspective in vec3 GEdgeDistance;
in vec4 ColorToFrag;
flat in uvec3 VertexFlagsFrag;
out vec4 FragColor;
bool drawEdge(uint v){
    return (v&2u)!=2u;
}
bool selected(uint v){
    return (v&1u)==1u;
}

void main() {
    if(Line.isDrawn==1){
	bool sel = false;
	float d = 10.0f;//more than line width
	if(drawEdge(VertexFlagsFrag.x) && GEdgeDistance.x < d){
	    d = GEdgeDistance.x;
	    sel = selected(VertexFlagsFrag.x);
	}
	if(drawEdge(VertexFlagsFrag.y) && GEdgeDistance.y < d){
	    d = GEdgeDistance.y;
	    sel = selected(VertexFlagsFrag.y);
	}
	if(drawEdge(VertexFlagsFrag.z) && GEdgeDistance.z < d){
	    d = GEdgeDistance.z;
	    sel = selected(VertexFlagsFrag.z);
	}
	if(sel){
	    float mixVal = smoothstep(Line.SelectedWidth -1, Line.SelectedWidth +1, d);
	    FragColor = mix( Line.SelectedColor,ColorToFrag , mixVal );
	}
	else{
	    float mixVal = smoothstep(Line.Width -1, Line.Width +1, d);
	    FragColor = mix( Line.Color,ColorToFrag , mixVal );
	}
    }
    else
	FragColor = ColorToFrag;
}
