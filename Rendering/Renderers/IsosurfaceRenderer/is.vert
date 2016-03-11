#version 400

in vec3 VertexPosition;
//in uint VertexFlags;
in float VertexScalar;
in vec3 VertexNormal;

//uniform int WireFrameOption;
uniform mat4 MVP;

out float ScalarValue;
out vec3 Normal;

void main()
{
    gl_Position = MVP * vec4(VertexPosition,1);
    ScalarValue = VertexScalar;
    Normal = VertexNormal;

    //vdata.VertexPositionWS = VertexPosition;
    //if(SelectedScalarPropIndex != -1) {
    //    vdata.ScalarValue = VertexScalars[SelectedScalarPropIndex];
    //} else {
    //    vdata.ScalarValue = 0.0;
    //}
    //vdata.VertexFlags = VertexFlags;

}
