#version 400
in vec4 VertexColor;
noperspective in float discardThisVertex;
out vec4 FragColor;
void main() {
    if(discardThisVertex<1.0f)
	discard;
    FragColor = VertexColor;
}
