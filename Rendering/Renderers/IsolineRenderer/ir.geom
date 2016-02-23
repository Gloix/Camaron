#version 400

struct VertexData{
    vec4 VertexPosition;
    vec4 WSVertexPosition;
    uint VertexFlags;
    float ScalarValue;
};
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexData vdata[3];
//out vec4 fcolor;
smooth out float scalarValue;
out float slope;

uniform vec4 WireFrameColor;
uniform int WireFrameOption;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = -sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

float calcSlope(vec3 points[3])
{
    vec3 normal = normalize(cross(points[1]-points[0], points[2]-points[0]));
    float projection = dot(normal, vec3(0.0,0.0,1.0));
    return tan(acos(projection));
}

void main()
{
    vec3 normal = normalize(cross(vdata[1].WSVertexPosition.xyz-vdata[0].WSVertexPosition.xyz,
                                   vdata[2].WSVertexPosition.xyz-vdata[0].WSVertexPosition.xyz));
    vec3 slopePoints[3];
    if(normal == vec3(0.0,0.0,1.0)) {
        slopePoints = vec3[3](vec3(vdata[0].WSVertexPosition.xy, vdata[0].ScalarValue),
                              vec3(vdata[1].WSVertexPosition.xy, vdata[1].ScalarValue),
                              vec3(vdata[2].WSVertexPosition.xy, vdata[2].ScalarValue));
    } else {
        float angle = acos(dot(normal,vec3(0.0,0.0,1.0)));
        vec3 rotationAxis = cross(normal,vec3(0.0,0.0,1.0));
        mat4 rotationMatrix = rotationMatrix(rotationAxis, angle);
        vec4 p0rot = rotationMatrix*vdata[0].WSVertexPosition;
        vec4 p1rot = rotationMatrix*vdata[1].WSVertexPosition;
        vec4 p2rot = rotationMatrix*vdata[2].WSVertexPosition;
        slopePoints = vec3[3](vec3(p0rot.xy, vdata[0].ScalarValue),
                           vec3(p1rot.xy, vdata[1].ScalarValue),
                           vec3(p2rot.xy, vdata[2].ScalarValue));
    }
    float slope_ = calcSlope(slopePoints);
    //float slope_ = 1.0;


    //triangle
    if((vdata[0].VertexFlags&8u)==8u){
        for (int i = 0; i < 3; i++)
        {
            gl_Position = vdata[i].VertexPosition;
            scalarValue = vdata[i].ScalarValue;
            slope = slope_;
            EmitVertex();
        }
        EndPrimitive();
    }
}

