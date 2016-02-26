#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData{
    vec4 position;
    float scalarIntensity;
    uint vertexFlags;
} vertexData[3];

out float scalarIntensity;

uniform int ElementDrawOption;

bool isFlagEnabled(uint f){
    return (vertexData[0].vertexFlags&f)==f &&
           (vertexData[1].vertexFlags&f)==f &&
           (vertexData[2].vertexFlags&f)==f;
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
    gl_Position = vertexData[0].position;
    scalarIntensity = vertexData[0].scalarIntensity;
    EmitVertex();
    gl_Position = vertexData[1].position;
    scalarIntensity = vertexData[1].scalarIntensity;
    EmitVertex();
    gl_Position = vertexData[2].position;
    scalarIntensity = vertexData[2].scalarIntensity;
    EmitVertex();
    EndPrimitive();
}

