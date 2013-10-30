#version 400

in vec4 VertexPosition;
in uint flags;
uniform vec4 ModelBaseColor;
uniform vec4 SelectedElementColor;
uniform mat4 MV;
uniform mat4 P;//Projection Matrix
out vec4 Color;
out uint VFlags;
out vec4 GeomVertexPosition;
out int GeomVertexInside;


uniform sampler1D Planes;
uniform int nPlanes;
uniform int realTimeCalc;
uniform int isSphere;
uniform int invertInside;
uniform float sphereRadio;
uniform vec3 sphereCenter;
uniform vec4 ousideElementBaseColor;
uniform vec4 ousideElementSelectedColor;


int isLeftPlanes(vec4);
int isInsideSphere(vec4);

void main()
{
   vec4 mvVertexPosition = MV*VertexPosition;
    if(realTimeCalc==1){
	if(isSphere!=1){
	    GeomVertexInside = isLeftPlanes(mvVertexPosition);
	}else{
	    GeomVertexInside = isInsideSphere(mvVertexPosition);
	}
	if(invertInside==1){
	    if(GeomVertexInside == 0)
		GeomVertexInside = 1;
	    else
		GeomVertexInside = 0;
	}
    }else{
	if((flags&64u)==64u)
	    GeomVertexInside = 1;
	else
	    GeomVertexInside = 0;
    }

    Color = ModelBaseColor;
    if((flags&1u)==1u){
	if(GeomVertexInside==1)
	    Color = SelectedElementColor;
	else
	    Color = ousideElementSelectedColor;
    }else{
	if(GeomVertexInside==1)
	    Color = ModelBaseColor;
	else
	    Color = ousideElementBaseColor;
    }
    VFlags = flags;
    GeomVertexPosition = P*mvVertexPosition;
}
