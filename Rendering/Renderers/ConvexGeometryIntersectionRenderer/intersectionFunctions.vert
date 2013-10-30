
bool isRight(vec3 planeOrigin, vec3 planeNormal, vec3 vposition){
    return dot(vposition-planeOrigin,planeNormal)<0.0;
}

int isInsideSphere(vec4 vposition){
    if(distance(sphereCenter,vposition.xyz)>sphereRadio)
	return 0;
    else
	return 1;
}

int isLeftPlanes(vec4 vposition){
    for(int i = 0;i<nPlanes;i++){
	vec3 planeNormal = vec3(texelFetch( Planes,i*2,0));
	vec4 planeOrigin = texelFetch( Planes, i*2+1,0 );
	if(isRight(planeOrigin.xyz,planeNormal,vec3(vposition/vposition.w)))
	    return 0;
    }
    return 1;
}
