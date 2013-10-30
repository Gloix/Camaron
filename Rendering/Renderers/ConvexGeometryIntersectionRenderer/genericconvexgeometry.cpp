#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"
#include "Utils/shaderutils.h"

#define POSITION_ATTRIBUTE 1
GLuint GenericConvexGeometry::genericProgram = 0;
bool GenericConvexGeometry::failed = false;

GenericConvexGeometry::GenericConvexGeometry()
{
	triangleVerticesBufferObject = 0;
	normalsBufferObject = 0;
	nNormalsVertices = 0;
	nTrianglesVertices = 0;
}
GenericConvexGeometry::GenericConvexGeometry(ConvexGeometryIntersectionRendererConfig *c):
	config(c)
{
	triangleVerticesBufferObject = 0;
	normalsBufferObject = 0;
	nNormalsVertices = 0;
	nTrianglesVertices = 0;
}
GenericConvexGeometry::~GenericConvexGeometry()
{
	if(triangleVerticesBufferObject)
		glDeleteBuffers(1,&triangleVerticesBufferObject);
	if(normalsBufferObject)
		glDeleteBuffers(1,&normalsBufferObject);
}


GLuint GenericConvexGeometry::getGenericProgram(){
	if(!GenericConvexGeometry::genericProgram&&!failed){
		ShaderLoadingData vertexShaderData(GL_VERTEX_SHADER);
		vertexShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgircg.vert");
		ShaderLoadingData fragmentShaderData(GL_FRAGMENT_SHADER);
		fragmentShaderData.addFile("Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgircg.frag");

		std::vector<ShaderLoadingData> shaderList;
		shaderList.push_back(vertexShaderData);
		shaderList.push_back(fragmentShaderData);


		VertexAttributeBindingData positionAttr = {POSITION_ATTRIBUTE, "VertexPosition"};
		std::vector<VertexAttributeBindingData> attributeList;
		attributeList.push_back(positionAttr);

		GenericConvexGeometry::genericProgram = ShaderUtils::CreateProgram(shaderList,attributeList);
		if(GenericConvexGeometry::genericProgram == ShaderUtils::FAIL_CREATING_PROGRAM){
			GenericConvexGeometry::genericProgram = 0;
			GenericConvexGeometry::failed = true;
		}
	}
	return GenericConvexGeometry::genericProgram;

}

void GenericConvexGeometry::setUniformData(GLuint){}
void GenericConvexGeometry::render(glm::mat4 matrix){
	GLuint shaderProgram = getGenericProgram();
	if(!shaderProgram)
		return;
	glUseProgram(shaderProgram);
	glDepthMask(false);
	ShaderUtils::setUniform(shaderProgram, "MVP",matrix);
	ShaderUtils::setUniform(shaderProgram, "BaseColor",config->convexGeometryColor);
	//In the render function, bind to the vertex array object and call glDrawArrays to
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, triangleVerticesBufferObject);
	glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
						   (GLubyte *)NULL );
	glDrawArrays(GL_TRIANGLES, 0, nTrianglesVertices );
	//normals
	if(config->drawNormal && nNormalsVertices && normalsBufferObject){
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
		glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
							   (GLubyte *)NULL );
		ShaderUtils::setUniform(shaderProgram, "BaseColor",config->convexGeometryNormalColor);
		glDrawArrays(GL_LINES, 0, nNormalsVertices );
	}
	glDisableVertexAttribArray(POSITION_ATTRIBUTE); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDepthMask(true);
	glUseProgram(0);
}
