#version 400
in vec3 in_Position;
in float in_ScalarProp;

out float scalarIntensity;

uniform mat4 u_TransformMatrix;
uniform int inverse_intensity;
uniform float min_bound;
uniform float max_bound;

void main(void){
	gl_Position = u_TransformMatrix * vec4(in_Position, 1.0);
        scalarIntensity = (in_ScalarProp - min_bound)/(max_bound - min_bound);
        scalarIntensity = clamp(scalarIntensity, 0.0, 1.0);
        if (inverse_intensity==1) scalarIntensity= 1.0 - scalarIntensity;
}
