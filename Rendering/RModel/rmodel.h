#ifndef RMODEL_H
#define RMODEL_H
#include <vector>
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Rendering/RModel/rmodelpropertyfielddef.h"

class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class LightWeightVertexCloud;
class LightWeightPolygonMesh;
class LightWeightPolyhedronMesh;
//template <typename T> class RModelPropertyFieldDef;
class ScalarFieldDef;
class PropertyFieldDef;
class Model;

class RModel
{
	public:
		RModel();
		virtual ~RModel();

		void loadRModelData(VertexCloud*);
		void loadRModelData(PolygonMesh*);
		void loadRModelData(PolyhedronMesh*);
		void loadRModelData(LightWeightVertexCloud*);
		void loadRModelData(LightWeightPolygonMesh*);
		void loadRModelData(LightWeightPolyhedronMesh*);

		void loadVertexPolygonPolyhedronIds(PolygonMesh* mesh);
		void loadVertexPositionAndNormals(VertexCloud* model);
		void loadAdditionalEdges(VertexCloud* model);
		std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> loadPropertyField(std::shared_ptr<ScalarFieldDef>);
		void loadTetrahedronVertexIds(PolyhedronMesh* mesh);

		void refreshVertexAttributes(PolyhedronMesh*);
		void refreshSelectedElements();
		void deleteData();
		Model* getOriginalModel();
		int nVertices;
		int nAdditionalEdges;
		std::vector<RVertexFlagAttribute> vertexFlagsAttribute;
		std::vector<RVertexFlagAttribute> vertexGeoCenterFlagsAttribute;
		std::vector<RVertexFlagAttribute> polygonGeoCenterFlagsAttribute;
		std::vector<RVertexFlagAttribute> polyhedronGeoCenterFlagsAttribute;

		std::vector<float> bounds;

		int numberOfTetrahedrons;
		int numberOfTriangles;

		//std::vector<std::shared_ptr<RModelPropertyFieldDef<>>> &getRModelPropertyFieldDefs();

		static const GLuint NULL_BUFFER = 0;
		//Main Rendering Model
		GLuint positionDataBufferObject;
		GLuint vertexNormalDataBufferObject;
		GLuint vertexFlagsDataBufferObject;
		GLuint rmodelVertexPositionBufferObject;
		GLuint edgeVertexPositionsDataBufferObject;
		GLuint edgeColorDataBufferObject;
		//ids
		GLuint polygonPolyhedronIdsBufferObject;
		GLuint vertexIdsBufferObject;
		GLuint tetrahedronVertexIdsBufferObject;
		int getModelType();
		void setBackgroundColor(glm::vec4);
		glm::vec4 getBackgroundColor();
		void setRotation(glm::vec3);
		void setTranslation(glm::vec3);
		void setViewPortSize(glm::vec2,glm::vec2);
		void setScale(float);
		void setUseOrthoProjection(bool);
		void setPerspectiveValue(float newVal);
		glm::mat4 getMVP();
		glm::mat3 getNormalMatrix();
		glm::mat4 getMV();
		glm::mat4 getMVPVP();
		glm::mat4 getViewPortMatrix();
		glm::mat4 getOrthoProjectionMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getPerspectiveProjectionMatrix();
		glm::mat4 getModelToCameraSpaceMatrix();
		glm::mat4 getModelToCameraSpaceAndScaleMatrix();
		glm::vec2 getViewPortSize();
		glm::vec3& getCameraPosition();
		bool isOrthoProjectionInUse();
		void setViewMatrix(glm::mat4);
	private:
		std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> currentRModelPropertyFieldDef;
		//std::vector<std::shared_ptr<RModelPropertyFieldDef>> rModelPropertyFieldDefs;
		void copyPropertyFieldDefs(VertexCloud*);
		int modelType;
		glm::vec3 cameraPosition;
		glm::vec4 backgroundColor;
		//Position
		glm::vec3 rotation;
		glm::vec3 translation;
		glm::vec2 viewPortSize;
		glm::vec2 viewPortZ;
		float scale;
		glm::mat4 MV;//Model View
		glm::mat4 MVP;//Model View Projection
		glm::mat4 MVPVP;//Model View Projection ViewPort
		glm::mat4 viewportMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 orthoProjectionMatrix;
		glm::mat4 perspectiveProjectionMatrix;
		glm::mat3 normalMatrix;
		float perspectiveValue;
		glm::mat4 modelToCameraSpace;
		glm::mat4 modelToCameraScaleSpace;
		void copyModelBounds(Model* model);
		void freeRAMFromVideoCardBuffer();
		Model* originalModel;
		bool recalculateMVP;
		bool recalculateMV;
		bool recalculateMVPVP;
		bool recalculateNormal;
		bool recalculateModelCameraSpaceScale;
		bool useOrthoProjection;

};

#endif // RMODEL_H
