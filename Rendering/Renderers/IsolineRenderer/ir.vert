#version 400

in vec4 VertexPosition;
in uint VertexFlags;
in float[10] VertexScalars;

uniform int SelectedScalarPropIndex;
uniform int WireFrameOption;
uniform mat4 MVP;
struct VertexData{
    vec4 VertexPosition;
    vec4 VertexPositionWS;
    uint VertexFlags;
    float ScalarValue;
};

out VertexData vdata;

void main()
{
    vdata.VertexPosition = MVP*VertexPosition;
    vdata.VertexPositionWS = VertexPosition;
    if(SelectedScalarPropIndex != -1) {
        vdata.ScalarValue = VertexScalars[SelectedScalarPropIndex];
    } else {
        vdata.ScalarValue = 0.0;
    }
    vdata.VertexFlags = VertexFlags;

}
