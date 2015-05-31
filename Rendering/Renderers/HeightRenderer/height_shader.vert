#version 400
in vec3 in_Position;

out float height_value;

uniform mat4 u_TransformMatrix;
uniform int axis;
uniform int coloring_type;
uniform int inverse_intensity;
uniform float min_x;
uniform float min_y;
uniform float min_z;
uniform float max_x;
uniform float max_y;
uniform float max_z;

void main(void){
	gl_Position = u_TransformMatrix * vec4(in_Position, 1.0);
	if (axis==0) { /*x axis*/
		height_value = (in_Position.x - min_x)/(max_x - min_x);
	}
	else if(axis==1){ /*y axis*/
		height_value = (in_Position.y - min_y)/(max_y - min_y);
	}
	else{	 /*z axis*/
		height_value = (in_Position.z - min_z)/(max_z - min_z);
	}
	height_value = clamp(height_value, 0.0, 1.0);
	if (inverse_intensity==1) height_value = 1.0 - height_value;
}
