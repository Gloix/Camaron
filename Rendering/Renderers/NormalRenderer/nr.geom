#version 400

struct VertexData{
    vec4 VertexTailPosition;
    vec4 VertexHeadPosition;
    uint VertexFlags;
};
layout(triangles) in;
layout(line_strip, max_vertices = 10) out;

in VertexData vdata[3];
uniform vec4 NormalHeadColor;
uniform vec4 NormalTailColor;
out vec4 fcolor;
uniform vec4 WireFrameColor;
uniform int WireFrameOption;
void main()
{

    for (int i = 0; i < 3; i++)
    {
	if((vdata[i].VertexFlags&8u)==8u && (vdata[i].VertexFlags&4u)==4u){
	    gl_Position = vdata[i].VertexTailPosition;
	    fcolor = NormalTailColor;
	    EmitVertex();
	    gl_Position = vdata[i].VertexHeadPosition;
	    fcolor = NormalHeadColor;
	    EmitVertex();

	    EndPrimitive();
	}
    }
    //triangle
    if(WireFrameOption == 0){
	return;
    }
    else{
	bool draw = false;
	if(WireFrameOption==1)
	    draw = true;
	else if((vdata[0].VertexFlags&8u)==8u)
	    draw = true;

	if(draw){
	    for (int i = 0; i < 3; i++)
	    {
		gl_Position = vdata[i].VertexTailPosition;
		fcolor = WireFrameColor;
		EmitVertex();
	    }
	    gl_Position = vdata[0].VertexTailPosition;
	    fcolor = WireFrameColor;
	    EmitVertex();
	    EndPrimitive();
	}
    }
}

