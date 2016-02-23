#version 400
//#extension GL_EXT_gpu_shader4 : require

//Adaptation of implementation (http://www.icare3d.org/codes-and-projects/codes/opengl_geometry_shader_marching_cubes.html)

struct VertexData{
    vec3 VertexPosition;
    float VertexScalar;
    uint VertexFlags;
};

layout(lines_adjacency) in;
layout(points, max_vertices = 120) out;

//Triangles table texture
uniform isampler2D triTableTex;

in VertexData vertexData[4];
//in vec3 VertexPosition[4];
//in float VertexScalar[4];
out vec3 vertexPosition;
out float scalarValue;
uniform float[20] Isolevels;
uniform int IsolevelsSize;
uniform int ElementDrawOption;

vec3 vertexInterp(float val, vec3 pos1, float v1, vec3 pos2, float v2) {
    return mix(pos1, pos2, (val-v1)/(v2-v1));
}

int triTableValue(int i, int j){
    return texelFetch(triTableTex, ivec2(j, i),0).r;
}

bool isFlagEnabled(uint f){
    return (vertexData[0].VertexFlags&f)==f &&
           (vertexData[1].VertexFlags&f)==f &&
           (vertexData[2].VertexFlags&f)==f &&
           (vertexData[3].VertexFlags&f)==f;
}

bool primitiveIsDrawn(){
    if(ElementDrawOption == 3 && !isFlagEnabled(1u))
        return false;
    else if(ElementDrawOption == 4 && isFlagEnabled(1u))
        return false;
    else
        return true;
}

void main()
{
    if(!primitiveIsDrawn()) {
        return;
    }
    for (int i=0 ; i < IsolevelsSize ; i++) {
        float isolevel = Isolevels[i];
        int tetindex = 0;
        float tetVal0 = vertexData[0].VertexScalar;
        float tetVal1 = vertexData[1].VertexScalar;
        float tetVal2 = vertexData[2].VertexScalar;
        float tetVal3 = vertexData[3].VertexScalar;
        //Determine the index into the edge table which
        //tells us which vertices are inside of the surface
        tetindex = int(tetVal0 < isolevel);
        tetindex += int(tetVal1 < isolevel)*2;
        tetindex += int(tetVal2 < isolevel)*4;
        tetindex += int(tetVal3 < isolevel)*8;
        //Tetrahedron is entirely in/out of the surface

        if (tetindex == 0 || tetindex == 15)
        continue;
        vec3 vertlist[6];
        //Find the vertices where the surface intersects the cube
        vertlist[0] = vertexInterp(isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[0].VertexPosition, tetVal0);
        vertlist[1] = vertexInterp(isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[2].VertexPosition, tetVal2);
        vertlist[2] = vertexInterp(isolevel, vertexData[0].VertexPosition, tetVal0, vertexData[2].VertexPosition, tetVal2);
        vertlist[3] = vertexInterp(isolevel, vertexData[0].VertexPosition, tetVal0, vertexData[3].VertexPosition, tetVal3);
        vertlist[4] = vertexInterp(isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[3].VertexPosition, tetVal3);
        vertlist[5] = vertexInterp(isolevel, vertexData[2].VertexPosition, tetVal2, vertexData[3].VertexPosition, tetVal3);

        int j=0;
        //Strange bug with this way, uncomment to test
        //for (j=0; triTableValue(cubeindex, j)!=-1; j+=3) {
        while(true){
            if(triTableValue(tetindex, j)!=-1 && j != 6){
                //if(triTableValue(tetindex, j) == 3 && triTableValue(tetindex, j+1) == 4 && triTableValue(tetindex, j+2) == 5) {
                //if(false && tetindex==7) {
                //    vertexPosition = vec3(0.0);
                //    scalarValue = 1;
                //    EmitVertex();
                //    vertexPosition = vec3(0.0,10.0,0.0);
                //    scalarValue = 2;
                //    EmitVertex();
                //    vertexPosition = vec3(0.0,0.0,10.0);
                //    scalarValue = 3;
                //    EmitVertex();
                //    EndPrimitive();
                //}
                //j=j+3;
                //continue;
                //Generate first vertex of triangle//
                //Fill position varying attribute for fragment shader
                vertexPosition = vertlist[triTableValue(tetindex, j)];
                //vertexPosition = vertlist[0];
                scalarValue = isolevel;
                EmitVertex();
                //Generate second vertex of triangle//
                //Fill position varying attribute for fragment shader
                vertexPosition = vertlist[triTableValue(tetindex, j+1)];
                //vertexPosition = vertlist[1];
                scalarValue = isolevel;
                EmitVertex();
                //Generate last vertex of triangle//
                //Fill position varying attribute for fragment shader
                vertexPosition = vertlist[triTableValue(tetindex, j+2)];
                //vertexPosition = vertlist[4];
                scalarValue = isolevel;
                EmitVertex();
                //End triangle strip at firts triangle
                EndPrimitive();
            }else{
                break;
            }
            j=j+3; //Comment it for testing the strange bug
        }
    }
}

