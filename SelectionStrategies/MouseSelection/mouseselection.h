#ifndef MOUSESELECTION_H
#define MOUSESELECTION_H
#include "SelectionStrategies/SelectionStrategy.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "SelectionStrategies/MouseSelection/mouseselectionconfig.h"
namespace vis{
class Element;
}
class FrameBufferObjectHandle;
class MouseSelection: public SelectionStrategy
{
	public:
		MouseSelection();
		~MouseSelection();
		void setRectCorners(glm::vec2, glm::vec2);
		void setViewPortSize(glm::vec2);
		void setPointSelection(glm::vec2);

		//From SelectionStrategy
		virtual bool selectElement( vis::Polyhedron *, Selection* );
		virtual bool selectElement( vis::Polygon *, Selection* );
		virtual bool selectElement( vis::Vertex *, Selection* );
		//depends on the evaluation strategy
		virtual bool isFullFilled( vis::Vertex * v );
		virtual bool isFullFilled( vis::Polygon * m );
		virtual bool isFullFilled( vis::Polyhedron * m );
		virtual bool isFullFilled( Selection * s);
		bool setupPreApplying(Selection*, RModel *rmodel);
		virtual bool setup();
		virtual bool hasConfigWidget();
		virtual QWidget* getConfigWidget();
		bool applyButtonEnabled();
		void setMVPVP(glm::mat4);
		void setVP(glm::mat4);
		void setMVP(glm::mat4);
		void setNormalMatrix(glm::mat3);
		void setUseOrthoProjection(bool);
	private:
		bool useOrthoProjection;
		glm::mat4 mvpvp;
		glm::mat4 mvp;
		glm::mat4 vp;
		glm::mat3 normalMatrix;
		glm::vec2 start;
		glm::vec2 end;
		glm::vec2 viewPortSize;
		glm::vec3 cameraPosition;
		bool rectSelection;//False: point selection
		bool createComputeShader();
		bool createFBO();
		bool createFBOShader();
		bool selectElement( vis::Element *, int elementType, Selection*);
		bool selectElementAlgorithm( vis::Element *, int elementType);
		//Select Using CPU
		bool selectCPU( vis::Polyhedron *);
		bool selectCPU( vis::Polygon * );
		bool selectCPU( vis::Vertex *);
		bool selectFBO( vis::Element *, int elementType);
		//Select Using CPU
		bool selectFBO( vis::Polyhedron *);
		bool selectFBO( vis::Polygon * );
		bool selectFBO( vis::Vertex *);
		inline bool isElementSelected( int id);
		inline glm::vec2 getViewPortCoords(glm::vec3);
		bool runRenderSelectionToTextureShader(Selection*, RModel*);
		//help
		bool hasFaceFacingCamera(vis::Polyhedron *p);
		vis::Polygon* getFaceFacingCamera(vis::Polyhedron *p);
		bool pointInsidePolygon(vis::Polygon *);
		bool COMPUTE_SHADER_AVAILABLE_SELECTION_AVAILABLE;
		bool RENDER_TEXTURE_SHADER_SELECTION_AVAILABLE;
		//Mouse selection config
		MouseSelectionConfig config;

		//Computer shader
		GLuint theProgram;

		//Render Buffer Elements
		std::vector<int> renderBufferSideSizes;
		static const unsigned char UNSELECTED_ELEMENT = 100;
		GLuint theProgramRenderTexture;
		FrameBufferObjectHandle* fboHandle;
		int memBufferSize;
		unsigned int* memBuffer;
		void resizeBuffer(int n);
		void clearBuffer();
		void getSelectedElements();

};
bool MouseSelection::isElementSelected( int id){
	//return memBuffer[id] != UNSELECTED_ELEMENT;
	int mask = 1<<(id%32);
	return memBuffer[id/32]&mask ;
}
glm::vec2 MouseSelection::getViewPortCoords(glm::vec3 modelCoords){
	glm::vec2 viewportcoords;
	if(useOrthoProjection)
		viewportcoords = glm::vec2(mvpvp*glm::vec4(modelCoords,1.0f));
	else{
		glm::vec4 mvpCoord = mvp*glm::vec4(modelCoords,1.0f);
		mvpCoord /= mvpCoord.w;
		viewportcoords = glm::vec2(vp*mvpCoord);
	}
	viewportcoords.y = viewPortSize.y-viewportcoords.y;
	return viewportcoords;
}

#endif // MOUSESELECTION_H
