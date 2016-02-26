#version 400

in vec3 VertexPosition;
in float VertexScalar;
in uint VertexFlags;

out VertexData{
    vec4 position;
    float scalarIntensity;
    uint vertexFlags;
} vdata;

uniform mat4 MVP;
uniform int inverse_intensity;
uniform float min_bound;
uniform float max_bound;

void main(){
        vdata.position = MVP*vec4(VertexPosition,1.0);
        vdata.scalarIntensity = 1.0;
        vdata.scalarIntensity = (VertexScalar - min_bound)/(max_bound - min_bound);
        vdata.scalarIntensity = clamp(vdata.scalarIntensity, 0.0, 1.0);
        if (inverse_intensity==1) vdata.scalarIntensity= 1.0 - vdata.scalarIntensity;
        vdata.vertexFlags = VertexFlags;
}
