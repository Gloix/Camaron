#version 400

in vec3 VertexPosition;
in uint VertexFlags;
in float VertexScalar;

uniform int WireFrameOption;
uniform mat4 MVP;
out VertexData{
    vec4 VertexPosition;
    vec3 VertexPositionWS;
    uint VertexFlags;
    float ScalarValue;
} vdata;

void main()
{
    vdata.VertexPosition = MVP*vec4(VertexPosition,1);
    vdata.VertexPositionWS = VertexPosition;
    vdata.ScalarValue = VertexScalar;
    vdata.VertexFlags = VertexFlags;

}
