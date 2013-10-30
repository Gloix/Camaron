#version 400
struct LineInfo {
    int Width;
    vec4 Color;
    int isDrawn; //1 = wire, 0 = no wire
};
uniform LineInfo Line;
uniform int stageNumber;
uniform sampler2D TexColorsDepth;
uniform vec2 ViewportSize;
noperspective in vec3 GEdgeDistance;
in vec3 IgnoreEdges;
in vec4 ColorToFrag;
out vec4 FragColor;

void main() {
    if(stageNumber!=0){
	vec4 texColorDept = texture( TexColorsDepth,vec2((gl_FragCoord.x-1.0f)/ViewportSize.x,
							 (gl_FragCoord.y-1.0f)/ViewportSize.y));
	if(gl_FragCoord.z<=(texColorDept.x+0.005f)){//bias = 0.005f
	    discard;
	}
    }
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
    FragColor.rgb = FragColor.rgb*FragColor.a;
}
