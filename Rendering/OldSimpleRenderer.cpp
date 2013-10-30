#include "Rendering/OldSimpleRenderer.h"
#include "Model/PolyhedronMesh.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include "GL/glew.h"
#include "Rendering/RModel/rmodel.h"
OldSimpleRenderer::OldSimpleRenderer():Renderer("Fixed Pipeline"){

}
OldSimpleRenderer::~OldSimpleRenderer(){

}

void OldSimpleRenderer::setUpDrawing(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef QT_OPENGL_ES_1
	glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
	glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
	glMatrixMode(GL_MODELVIEW);
}
void OldSimpleRenderer::draw(RModel*){
	//glTranslatef(0.0, 0.0, -10.0);
	/*glm::mat4 modelToCamera = rmodel->modelToCameraSpace;
	glScalef(modelToCamera[0].x,modelToCamera[0].x,modelToCamera[0].x );
	glRotatef(rmodel->rotation.x / PI*180.0f, 1.0, 0.0, 0.0);
	glRotatef(rmodel->rotation.y / PI*180.0f, 0.0, 1.0, 0.0);
	glRotatef(rmodel->rotation.z / PI*180.0f, 0.0, 0.0, 1.0);
	glScalef(rmodel->scale,rmodel->scale,rmodel->scale);
	std::vector<glm::vec3> &vertices = rmodel->vertices;
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f,0.0f,0.0f);
	for( std::vector<GLuint>::size_type i = 0; i != vertices.size(); i++ ){
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();
	std::vector<GLuint>::size_type ntriangles = vertices.size()/3;
	for( std::vector<GLuint>::size_type i = 0; i != ntriangles; i++ ){
		int triangleIndex = 3*i;
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0,0.0, 0.0);
		glVertex3f(vertices[triangleIndex].x, vertices[triangleIndex].y, vertices[triangleIndex].z);
		glVertex3f(vertices[triangleIndex+1].x, vertices[triangleIndex+1].y, vertices[triangleIndex+1].z);
		glVertex3f(vertices[triangleIndex+2].x, vertices[triangleIndex+2].y, vertices[triangleIndex+2].z);
		glVertex3f(vertices[triangleIndex].x, vertices[triangleIndex].y, vertices[triangleIndex].z);
		glEnd();
	}*/
}
