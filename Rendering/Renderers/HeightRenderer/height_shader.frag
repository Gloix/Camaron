#version 400
out vec4 out_Color;

in float height_value;

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

vec4 hsv_to_rgb(float h, float s, float v, float a)
{
	float c = v * s;
	h = mod((h * 6.0), 6.0);
	float x = c * (1.0 - abs(mod(h, 2.0) - 1.0));
	vec4 color;
 
	if (0.0 <= h && h < 1.0) {
		color = vec4(c, x, 0.0, a);
	} else if (1.0 <= h && h < 2.0) {
		color = vec4(x, c, 0.0, a);
	} else if (2.0 <= h && h < 3.0) {
		color = vec4(0.0, c, x, a);
	} else if (3.0 <= h && h < 4.0) {
		color = vec4(0.0, x, c, a);
	} else if (4.0 <= h && h < 5.0) {
		color = vec4(x, 0.0, c, a);
	} else if (5.0 <= h && h < 6.0) {
		color = vec4(c, 0.0, x, a);
	} else {
		color = vec4(0.0, 0.0, 0.0, a);
	}
 
	color.rgb += v - c;
 
	return color;
}

void main(void){
        float v = clamp(height_value, 0.0, 1.0);
	vec4 col=vec4(1.0, 0.0, 0.0, 1.0);
	if (coloring_type == 0){ /*monochromatic*/
		col = vec4(v, v, v, 1.0);
	}
	else{
                v = v * 2.0 / 3.0; /*when want the color scale from red to blue (not red-red)*/
                col = hsv_to_rgb(v, 1.0, 1.0, 1.0);
	}
	out_Color = col;
}
