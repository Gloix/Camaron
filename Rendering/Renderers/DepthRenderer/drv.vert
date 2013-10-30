#version 400
in vec4 VertexPosition;
out vec2 FragTexCoord;
void main()
{
    FragTexCoord = vec2(1.0f,1.0f);
    if(VertexPosition.x == -1.0){
	FragTexCoord.x = 0.0;
    }
    if(VertexPosition.y == -1.0){
	FragTexCoord.y = 0.0;
    }
    gl_Position = VertexPosition;
}
