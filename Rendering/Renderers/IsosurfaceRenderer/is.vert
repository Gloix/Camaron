#version 400

in vec3 VertexPosition;
//in uint VertexFlags;
in float VertexScalar;

//uniform int WireFrameOption;
uniform mat4 MVP;

out float ScalarValue;

void main()
{
    gl_Position = MVP * vec4(VertexPosition,1);
    ScalarValue = VertexScalar;

    //vdata.VertexPositionWS = VertexPosition;
    //if(SelectedScalarPropIndex != -1) {
    //    vdata.ScalarValue = VertexScalars[SelectedScalarPropIndex];
    //} else {
    //    vdata.ScalarValue = 0.0;
    //}
    //vdata.VertexFlags = VertexFlags;

}
