#version 400

in VertexData{
    vec3 Position;
    uint Flags;
    float ScalarValue;
} vdata[3];
layout(triangles) in;
layout(points, max_vertices = 23) out;

out vec3 vertexPosition;
out float scalarValue;
uniform float[20] Isolevels;
uniform int IsolevelsSize;
uniform float ScalarMin;
uniform float ScalarMax;

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

        vec3 vertlist[3];
        //Find the vertices where the surface intersects the triangle
        vertlist[0] = vertexInterp(isolevel, vdata[1].Position, triVal1, vdata[0].Position, triVal0);
        vertlist[1] = vertexInterp(isolevel, vdata[1].Position, triVal1, vdata[2].Position, triVal2);
        vertlist[2] = vertexInterp(isolevel, vdata[0].Position, triVal0, vdata[2].Position, triVal2);
        vec4 linecolor;

        if(triindex == 1 || triindex == 6) {
            vertexPosition = vertlist[0];
            scalarValue = isolevel;
            EmitVertex();
            vertexPosition = vertlist[2];
            scalarValue = isolevel;
            EmitVertex();
            EndPrimitive();
        } else if(triindex == 2 || triindex == 5) {
            vertexPosition = vertlist[0];
            scalarValue = isolevel;
            EmitVertex();
            vertexPosition = vertlist[1];
            scalarValue = isolevel;
            EmitVertex();
            EndPrimitive();
        } else if(triindex == 3 || triindex == 4) {
            vertexPosition = vertlist[1];
            scalarValue = isolevel;
            EmitVertex();
            vertexPosition = vertlist[2];
            scalarValue = isolevel;
            EmitVertex();
            EndPrimitive();
        }
    }
}

