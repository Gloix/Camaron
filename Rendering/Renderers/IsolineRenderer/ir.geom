#version 400

struct VertexData{
    vec4 VertexPosition;
    vec3 VertexPositionWS;
    uint VertexFlags;
    float ScalarValue;
};
layout(triangles) in;
layout(line_strip, max_vertices = 23) out;

in VertexData vdata[3];
out vec4 fcolor;
uniform vec4 WireFrameColor;
uniform int WireFrameOption;
uniform mat4 MVP;
uniform float[20] Isolevels;
uniform int IsolevelsSize;
uniform float ScalarMin;
uniform float ScalarMax;

uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;

//void isolineVertex(float v1, float v2, vec4 pos1, vec4 pos2, float val) {
//    if(v1 == v2) return;
//    float mult = (val - v1)/(v2 - v1);
//    if( mult >= 0.0 && mult <= 1.0 ) {
//        gl_Position = MVP*(pos1 + mult*(pos2 - pos1));
//        float colorMult = (val-IsolinesSteps[0])/(IsolinesSteps[IsolinesStepsN-1]-IsolinesSteps[0]);
//        fcolor = mix(GradientStartColor, GradientEndColor, colorMult);
//        EmitVertex();
//    }
//}

vec3 vertexInterp(float val, vec3 pos1, float v1, vec3 pos2, float v2) {
    return mix(pos1, pos2, (val-v1)/(v2-v1));
}

void main()
{
    float triVal0 = vdata[0].ScalarValue;
    float triVal1 = vdata[1].ScalarValue;
    float triVal2 = vdata[2].ScalarValue;

    for (int i=0 ; i < IsolevelsSize ; i++) {

        float isolevel = Isolevels[i];
        int triindex = 0;

        //Determine the index into the edge table which
        //tells us which vertices are inside of the surface
        triindex = int(triVal0 < isolevel);
        triindex += int(triVal1 < isolevel)*2;
        triindex += int(triVal2 < isolevel)*4;

        //Triangle is entirely in/out of the surface
        if (triindex == 0 || triindex == 7)
        continue;

        vec4 vertlist[3];
        //Find the vertices where the surface intersects the triangle
        vertlist[0] = vec4(vertexInterp(isolevel, vdata[1].VertexPositionWS, triVal1, vdata[0].VertexPositionWS, triVal0),1);
        vertlist[1] = vec4(vertexInterp(isolevel, vdata[1].VertexPositionWS, triVal1, vdata[2].VertexPositionWS, triVal2),1);
        vertlist[2] = vec4(vertexInterp(isolevel, vdata[0].VertexPositionWS, triVal0, vdata[2].VertexPositionWS, triVal2),1);
        vec4 linecolor;
        linecolor = mix(GradientStartColor, GradientEndColor,
                  (isolevel-ScalarMin)/(ScalarMax-ScalarMin));
        if(triindex == 1 || triindex == 6) {
            gl_Position = MVP*vertlist[0];
            fcolor = linecolor;
            EmitVertex();
            gl_Position = MVP*vertlist[2];
            fcolor = linecolor;
            EmitVertex();
            EndPrimitive();
        } else if(triindex == 2 || triindex == 5) {
            gl_Position = MVP*vertlist[0];
            fcolor = linecolor;
            EmitVertex();
            gl_Position = MVP*vertlist[1];
            fcolor = linecolor;
            EmitVertex();
            EndPrimitive();
        } else if(triindex == 3 || triindex == 4) {
            gl_Position = MVP*vertlist[1];
            fcolor = linecolor;
            EmitVertex();
            gl_Position = MVP*vertlist[2];
            fcolor = linecolor;
            EmitVertex();
            EndPrimitive();
        }
    }
    return;
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
                gl_Position = vdata[i].VertexPosition;
                fcolor = WireFrameColor;
                EmitVertex();
            }
            gl_Position = vdata[0].VertexPosition;
            fcolor = WireFrameColor;
            EmitVertex();
            EndPrimitive();
        }
    }
}

