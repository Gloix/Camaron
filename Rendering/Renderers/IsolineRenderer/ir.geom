#version 400

struct VertexData{
    vec4 VertexPosition;
    vec4 VertexPositionWS;
    uint VertexFlags;
    float ScalarValue;
};
layout(triangles) in;
layout(line_strip, max_vertices = 105) out;

in VertexData vdata[3];
uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;
out vec4 fcolor;
uniform vec4 WireFrameColor;
uniform int WireFrameOption;
uniform mat4 MVP;
uniform float[50] IsolinesSteps;
uniform int IsolinesStepsN;

void isolineVertex(float v1, float v2, vec4 pos1, vec4 pos2, float val) {
    if(v1 == v2) return;
    float mult = (val - v1)/(v2 - v1);
    if( mult >= 0.0 && mult <= 1.0 ) {
        gl_Position = MVP*(pos1 + mult*(pos2 - pos1));
        float colorMult = (val-IsolinesSteps[0])/(IsolinesSteps[IsolinesStepsN-1]-IsolinesSteps[0]);
        fcolor = mix(GradientStartColor, GradientEndColor, colorMult);
        EmitVertex();
    }
}

void main()
{
    float v0 = vdata[0].ScalarValue;
    float v1 = vdata[1].ScalarValue;
    float v2 = vdata[2].ScalarValue;
    vec3 vdatavec = vec3(v0, v1, v2);

    for (int i=0 ; i < IsolinesStepsN ; i++) {
        float val = IsolinesSteps[i];

        if(all(greaterThan(vdatavec, vec3(val))) || all(greaterThan(vec3(val),vdatavec))) continue;
        //if((v0 < val && v1 < val && v2 < val) || (v0 > val && v1 > val && v2 > val)) continue;

        isolineVertex(v0, v1, vdata[0].VertexPositionWS, vdata[1].VertexPositionWS, val);
        isolineVertex(v1, v2, vdata[1].VertexPositionWS, vdata[2].VertexPositionWS, val);
        isolineVertex(v2, v0, vdata[2].VertexPositionWS, vdata[0].VertexPositionWS, val);
        EndPrimitive();

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

