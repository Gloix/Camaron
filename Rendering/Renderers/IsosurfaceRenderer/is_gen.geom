#version 400

//Adaptation of implementation (http://www.icare3d.org/codes-and-projects/codes/opengl_geometry_shader_marching_cubes.html)

in VertexData{
    vec3 VertexPosition;
    float VertexScalar;
    uint VertexFlags;
} vertexData[4];

layout(lines_adjacency) in;
layout(points, max_vertices = 6) out;

//Triangles table texture
uniform isampler2D triTableTex;

out vec3 vertexPosition;
out vec3 vertexNormal;

uniform float Isolevel;
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
    int tetindex = 0;
    float tetVal0 = vertexData[0].VertexScalar;
    float tetVal1 = vertexData[1].VertexScalar;
    float tetVal2 = vertexData[2].VertexScalar;
    float tetVal3 = vertexData[3].VertexScalar;

    //Determine the index into the edge table which
    //tells us which vertices are inside of the surface
    tetindex = int(tetVal0 < Isolevel);
    tetindex += int(tetVal1 < Isolevel)*2;
    tetindex += int(tetVal2 < Isolevel)*4;
    tetindex += int(tetVal3 < Isolevel)*8;

    //Tetrahedron is entirely in/out of the surface
    if (tetindex == 0 || tetindex == 15) {
        return;
    }

    //Find the vertices where the surface intersects the tetrahedron
    vec3 vertlist[6];
    vertlist[0] = vertexInterp(Isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[0].VertexPosition, tetVal0);
    vertlist[1] = vertexInterp(Isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[2].VertexPosition, tetVal2);
    vertlist[2] = vertexInterp(Isolevel, vertexData[0].VertexPosition, tetVal0, vertexData[2].VertexPosition, tetVal2);
    vertlist[3] = vertexInterp(Isolevel, vertexData[0].VertexPosition, tetVal0, vertexData[3].VertexPosition, tetVal3);
    vertlist[4] = vertexInterp(Isolevel, vertexData[1].VertexPosition, tetVal1, vertexData[3].VertexPosition, tetVal3);
    vertlist[5] = vertexInterp(Isolevel, vertexData[2].VertexPosition, tetVal2, vertexData[3].VertexPosition, tetVal3);

    int j=0;
    //Strange bug with this way, uncomment to test
    //for (j=0; triTableValue(cubeindex, j)!=-1; j+=3) {
    while(true){
        if(triTableValue(tetindex, j)!=-1 && j != 6){
            vec3[3] vertexPositions;
            vertexPositions[0] = vertlist[triTableValue(tetindex, j)];
            vertexPositions[1] = vertlist[triTableValue(tetindex, j+1)];
            vertexPositions[2] = vertlist[triTableValue(tetindex, j+2)];
            vec3 normal = normalize(cross(vertexPositions[1]-vertexPositions[0],
                                    vertexPositions[2]-vertexPositions[0]));

            //Generate first vertex of triangle//
            //Fill position varying attribute for fragment shader
            vertexPosition = vertexPositions[0];
            vertexNormal = normal;
            EmitVertex();
            EndPrimitive();

            //Generate second vertex of triangle//
            //Fill position varying attribute for fragment shader
            vertexPosition = vertexPositions[1];
            vertexNormal = normal;
            EmitVertex();
            EndPrimitive();

            //Generate last vertex of triangle//
            //Fill position varying attribute for fragment shader
            vertexPosition = vertexPositions[2];
            vertexNormal = normal;
            EmitVertex();
            EndPrimitive();
        }else{
            break;
        }
        j=j+3; //Comment it for testing the strange bug
    }
}
