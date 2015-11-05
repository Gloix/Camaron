#include "Rendering/RModel/rmodel.h"
#include "Model/PolyhedronMesh.h"
#include "Model/PolygonMesh.h"
#include "Model/VertexCloud.h"
#include "Model/lightweightvertexcloud.h"
#include "Model/lightweightpolygonmesh.h"
#include "Model/lightweightpolyhedronmesh.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/edge.h"
#include "Utils/PolygonUtils.h"
#include "Utils/polyhedronutils.h"
#include "Utils/matrixtransformationutils.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"


RModel::RModel():
	cameraPosition(CAMERA_POSITION_X,
				   CAMERA_POSITION_Y,
				   CAMERA_POSITION_Z)
{
	positionDataBufferObject = NULL_BUFFER;
	vertexFlagsDataBufferObject = NULL_BUFFER;
	vertexNormalDataBufferObject = NULL_BUFFER;
	rmodelVertexPositionBufferObject = NULL_BUFFER;
    vertexScalarDataBufferObject = NULL_BUFFER;
	polygonPolyhedronIdsBufferObject = NULL_BUFFER;
    tetrahedronVertexIdsBufferObject = NULL_BUFFER;
    edgeVertexPositionsDataBufferObject = NULL_BUFFER;
    edgeColorDataBufferObject = NULL_BUFFER;
	scale = 1.0f;
	modelType = vis::CONSTANTS::NO_MODEL;
	originalModel = (Model*)0;
	recalculateMVP = true;
	recalculateMV = true;
	recalculateMVPVP = true;
	useOrthoProjection = true;
	recalculateNormal = true;
	recalculateModelCameraSpaceScale = true;
	perspectiveValue = 5.0f;
    numberOfTetrahedrons=0;
}
RModel::~RModel(){
	this->freeRAMFromVideoCardBuffer();
	for(std::vector<RModelVScalarDef*>::size_type i=0;i<scalarDefs.size();i++) {
		delete(scalarDefs[i]);
	}
}

void RModel::deleteData(){
	this->freeRAMFromVideoCardBuffer();
	vertexFlagsAttribute.clear();
	std::vector<RVertexFlagAttribute>().swap(vertexFlagsAttribute);
    scalarDefs.clear();
	bounds.resize(6);
	bounds[0] = 0.0;
	bounds[1] = 0.0;
	bounds[2] = 0.0;
	bounds[3] = 1.0;
	bounds[4] = 1.0;
	bounds[5] = 1.0;
	scale = 1.0f;
	originalModel = (Model*)0;
	modelType = vis::CONSTANTS::NO_MODEL;
}

Model* RModel::getOriginalModel(){
	return originalModel;
}

void RModel::loadRModelData(VertexCloud* model){
	this->freeRAMFromVideoCardBuffer();
	modelType = vis::CONSTANTS::VERTEX_CLOUD;
	std::vector<glm::vec3> vertices;
	vertices.reserve(model->getVertices().size());
	this->vertexFlagsAttribute.clear();
	this->vertexFlagsAttribute.reserve(model->getVertices().size());
	this->copyModelBounds(model);
	modelToCameraSpace = MatrixTransformationUtils::generateModelToCameraSpaceMatrix(model->getBounds());
	RVertexFlagAttribute flag;
	flag.disableAll();
	flag.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
	std::vector<vis::Vertex*> &modelVertices = model->getVertices();
	for( std::vector<vis::Vertex*>::size_type i = 0; i < modelVertices.size(); i++ ) {
		vertices.push_back(modelVertices[i]->getCoords());
		vertexFlagsAttribute.push_back(flag);
	}
	nVertices = vertices.size();
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vertices);
	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
    loadAdditionalEdges(model);
	originalModel = model;
	copyScalarDefs(model);
}

void RModel::copyScalarDefs(VertexCloud* model) {
	std::vector<VScalarDef*> modelScalarDefs = model->getScalarDefs();
	RModelVScalarDef* rmodelVScalarDef;
	for(std::vector<VScalarDef*>::size_type i = 0; i<modelScalarDefs.size(); i++ ) {
		rmodelVScalarDef = new RModelVScalarDef;
		rmodelVScalarDef->buffer = vertexScalarDataBufferObject;
		rmodelVScalarDef->bounds.assign(modelScalarDefs[i]->bounds.begin(), modelScalarDefs[i]->bounds.end());
		rmodelVScalarDef->name = std::string(modelScalarDefs[i]->name);
		rmodelVScalarDef->offset = modelScalarDefs[i]->index*sizeof(float);
		rmodelVScalarDef->stride = modelScalarDefs.size()*sizeof(float);
		scalarDefs.push_back(rmodelVScalarDef);
	}
	//X
	rmodelVScalarDef = new RModelVScalarDef;
	rmodelVScalarDef->buffer = positionDataBufferObject;
	rmodelVScalarDef->bounds.push_back(model->getBounds()[0]);
	rmodelVScalarDef->bounds.push_back(model->getBounds()[3]);
	rmodelVScalarDef->name = std::string("X");
	rmodelVScalarDef->offset = 0;
	rmodelVScalarDef->stride = 3*sizeof(float);
	scalarDefs.push_back(rmodelVScalarDef);
	//Y
	rmodelVScalarDef = new RModelVScalarDef;
	rmodelVScalarDef->buffer = positionDataBufferObject;
	rmodelVScalarDef->bounds.push_back(model->getBounds()[1]);
	rmodelVScalarDef->bounds.push_back(model->getBounds()[4]);
	rmodelVScalarDef->name = std::string("Y");
	rmodelVScalarDef->offset = 1*sizeof(float);
	rmodelVScalarDef->stride = 3*sizeof(float);
	scalarDefs.push_back(rmodelVScalarDef);
	//Z
	if(!model->is2D()) {
		rmodelVScalarDef = new RModelVScalarDef;
		rmodelVScalarDef->buffer = positionDataBufferObject;
		rmodelVScalarDef->bounds.push_back(model->getBounds()[2]);
		rmodelVScalarDef->bounds.push_back(model->getBounds()[5]);
		rmodelVScalarDef->name = std::string("Z");
		rmodelVScalarDef->offset = 2*sizeof(float);
		rmodelVScalarDef->stride = 3*sizeof(float);
		scalarDefs.push_back(rmodelVScalarDef);
	}
}

void RModel::copyModelBounds(Model* model){
	this->bounds.clear();
	this->bounds.reserve(model->getBounds().size());
	for(std::vector<float>::size_type i = 0; i< model->getBounds().size(); i++)
		this->bounds.push_back(model->getBounds()[i]);
}

void RModel::loadRModelData(PolygonMesh* mesh){
	this->freeRAMFromVideoCardBuffer();
	modelType = vis::CONSTANTS::POLYGON_MESH;
	vertexFlagsAttribute.clear();
	vertexFlagsAttribute.reserve(mesh->getVertices().size());
	copyModelBounds(mesh);
	modelToCameraSpace = MatrixTransformationUtils::generateModelToCameraSpaceMatrix(mesh->getBounds());
	std::vector<vis::Polygon*> &modelPolygons = mesh->getPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i < modelPolygons.size(); i++ )
		PolygonUtils::getTriangleVertices(modelPolygons[i],vertexFlagsAttribute);
	nVertices = vertexFlagsAttribute.size();
	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
	loadVertexPositionAndNormals(mesh);
	loadVertexPolygonPolyhedronIds(mesh);
    if(mesh->getAdditionalEdges().size() != 0) {
        loadAdditionalEdges(mesh);
    }
    std::cout << "Loading Scalar Properties" << std::endl;
    loadVertexScalarProperties(mesh);
	originalModel = mesh;
	copyScalarDefs(mesh);
}

void RModel::loadAdditionalEdges(VertexCloud* vcloud){
    std::vector<glm::vec3> edgeContainer;
    std::vector<glm::vec3> edgeColorContainer;
    std::vector<vis::Edge*>& additionalEdges = vcloud->getAdditionalEdges();
    edgeContainer.reserve(vcloud->getAdditionalEdgesCount()*2);
    edgeColorContainer.reserve(vcloud->getAdditionalEdgesCount()*2);

    for(std::vector<vis::Edge*>::size_type i = 0;i<additionalEdges.size();i++){
        vis::Edge* currentEdge = additionalEdges[i];
        glm::vec3& vec0 = currentEdge->getVertex0()->getCoords();
        glm::vec3& vec1 = currentEdge->getVertex1()->getCoords();
        glm::vec3& color = currentEdge->getColor();
        edgeContainer.push_back(vec0);
        edgeContainer.push_back(vec1);
        edgeColorContainer.push_back(color);
        edgeColorContainer.push_back(color);
    }
    nAdditionalEdges = vcloud->getAdditionalEdgesCount();
    edgeVertexPositionsDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(edgeContainer);
    edgeColorDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(edgeColorContainer);
}

/*void RModel::loadRModelData(PolygonMesh* mesh){
	this->freeRAMFromVideoCardBuffer();
	modelType = vis::CONSTANTS::POLYGON_MESH;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<int> polygonsPolyhedronsIds;
	vertices.reserve(mesh->getVertices().size());//min
	normals.reserve(mesh->getVertices().size());
	this->vertexFlagsAttribute.clear();
	this->vertexFlagsAttribute.reserve(mesh->getVertices().size());
	this->copyModelBounds(mesh);
	modelToCameraSpace = MatrixTransformationUtils::generateModelToCameraSpaceMatrix(mesh->getBounds());
	std::vector<vis::Polygon*> &modelPolygons = mesh->getPolygons();
	for( std::vector<vis::Polygon*>::size_type i = 0; i < modelPolygons.size(); i++ )
		PolygonUtils::getTriangleVertices(modelPolygons[i],vertices,
										  normals,this->vertexFlagsAttribute,
										  polygonsPolyhedronsIds);
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vertices);
	polygonPolyhedronIdsBufferObject = ShaderUtils::createDataBuffer<int>(polygonsPolyhedronsIds);
	vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(normals);
	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
	nVertices = vertices.size();
	for(int i = 0;i<nVertices;i++)
		polygonsPolyhedronsIds[i]=i;
	rmodelVertexPositionBufferObject = ShaderUtils::createDataBuffer<int>(polygonsPolyhedronsIds);
	std::vector<vis::Vertex*>& modelVertices = mesh->getVertices();
	for(std::vector<vis::Vertex*>::size_type i = 0;i<modelVertices.size();i++){
		std::vector<int>& rmodelPos = modelVertices[i]->getRmodelPositions();
		for(std::vector<int>::size_type j = 0;j<rmodelPos.size();j++){
			polygonsPolyhedronsIds[rmodelPos[j]]=modelVertices[i]->getId();
			//	polygonsPolyhedronsIds[rmodelPos[j]]=1000;
		}
	}
	vertexIdsBufferObject = ShaderUtils::createDataBuffer<int>(polygonsPolyhedronsIds);
	originalModel = mesh;
}//*/
void RModel::loadVertexPositionAndNormals(VertexCloud* model){
	std::vector<glm::vec3> vecContainer;
	vecContainer.resize(nVertices);
	std::vector<vis::Vertex*>& vertices = model->getVertices();
	//coords
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		vis::Vertex* currentVertex = vertices[i];
		std::vector<int>& rmodelPos = currentVertex->getRmodelPositions();
		for(std::vector<vis::Vertex*>::size_type j = 0;j<rmodelPos.size();j++)
			vecContainer[rmodelPos[j]] = currentVertex->getCoords();
	}
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vecContainer);
	//normals
	for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
		vis::Vertex* currentVertex = vertices[i];
		std::vector<int>& rmodelPos = currentVertex->getRmodelPositions();
		for(std::vector<int>::size_type j = 0;j<rmodelPos.size();j++)
			vecContainer[rmodelPos[j]] = currentVertex->getNormal();
	}
	vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vecContainer);

}

void RModel::loadVertexScalarProperties(VertexCloud* model){
    std::vector<float> floatContainer;
    std::vector<VScalarDef*>::size_type scalarDefsSize = model->getScalarDefs().size();
    floatContainer.resize(nVertices * scalarDefsSize);
    std::vector<vis::Vertex*>& vertices = model->getVertices();
    //coords
    for(std::vector<vis::Vertex*>::size_type i = 0;i<vertices.size();i++){
        vis::Vertex* currentVertex = vertices[i];
        //std::vector<VScalar> scalarProps = currentVertex->getScalarProperties();
        std::vector<int>& rmodelPos = currentVertex->getRmodelPositions();
        for(std::vector<vis::Vertex*>::size_type j = 0;j<rmodelPos.size();j++)
            for(std::vector<VScalarDef*>::size_type k = 0;k<scalarDefsSize;k++)
                floatContainer[rmodelPos[j]*scalarDefsSize+k] = currentVertex->getProperty(k);
    }
    vertexScalarDataBufferObject = ShaderUtils::createDataBuffer<float>(floatContainer);

}

void RModel::loadVertexPolygonPolyhedronIds(PolygonMesh* mesh){
	std::vector<int> ids;
	std::vector<vis::Polygon*>& polygons = mesh->getPolygons();
	/*
	std::vector<glm::vec3> vecContainer;
	vecContainer.resize(nVertices);
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();i++){
		vis::Polygon* currentVertex = polygons[i];
		std::vector<int>& rmodelPos = currentVertex->getRmodelPositions();
		for(std::vector<int>::size_type j = 0;j<rmodelPos.size();j++)
			vecContainer[rmodelPos[j]] = currentVertex->getNormal();
	}

	vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vecContainer);
//*/
	for(std::vector<vis::Polygon*>::size_type i = 0;i<polygons.size();++i){
		vis::Polygon* p = polygons[i];
		int npoltriangles = p->getRmodelPositions().size()/3;
		vis::Polyhedron** neighbors = p->getNeighborPolyhedron();
		int id0 = p->getId();
		int id1 = (neighbors[0])?neighbors[0]->getId():-1;
		int id2 = (neighbors[1])?neighbors[1]->getId():-1;
		for(int j = 0;j<npoltriangles;j++){
			ids.push_back(id0);
			ids.push_back(id1);
			ids.push_back(id2);
		}
	}
	polygonPolyhedronIdsBufferObject = ShaderUtils::createDataBuffer<int>(ids);
	for(int i = 0;i<nVertices;++i)
		ids[i]=i;
	rmodelVertexPositionBufferObject = ShaderUtils::createDataBuffer<int>(ids);
	std::vector<vis::Vertex*>& modelVertices = mesh->getVertices();
	for(std::vector<vis::Vertex*>::size_type i = 0;i<modelVertices.size();++i){
		std::vector<int>& rmodelPos = modelVertices[i]->getRmodelPositions();
		for(std::vector<int>::size_type j = 0;j<rmodelPos.size();j++){
			ids[rmodelPos[j]]=modelVertices[i]->getId();
			//polygonsPolyhedronsIds[rmodelPos[j]]=1000;
		}
	}
	vertexIdsBufferObject = ShaderUtils::createDataBuffer<int>(ids);
}

void RModel::loadTetrahedronVertexIds(PolyhedronMesh* mesh) {
    std::vector<GLuint> ids;
    std::vector<vis::Polyhedron*>& polyhedrons = mesh->getPolyhedrons();
    for(std::vector<vis::Polyhedron*>::size_type i=0;i<polyhedrons.size();i++ ) {
        PolyhedronUtils::getTetrahedronIndices(polyhedrons[i], ids);
    }
    numberOfTetrahedrons = ids.size()/4;
    tetrahedronVertexIdsBufferObject = ShaderUtils::createDataBuffer<GLuint>(ids);
}

void RModel::loadRModelData(PolyhedronMesh* mesh){
	this->loadRModelData((PolygonMesh*)mesh);
	modelType = vis::CONSTANTS::POLYHEDRON_MESH;
	std::vector<vis::Polyhedron*>& polyhedrons = mesh->getPolyhedrons();
	for(std::vector<vis::Polyhedron*>::size_type i = 0;i<polyhedrons.size();i++)
		PolyhedronUtils::setPolyhedronRModelPositions(polyhedrons[i]);
    loadTetrahedronVertexIds(mesh);
}
void RModel::loadRModelData(LightWeightVertexCloud* vcloud){
	this->freeRAMFromVideoCardBuffer();
	modelType = vis::CONSTANTS::LIGHT_WEIGHT_VERTEX_CLOUD;
	std::vector<glm::vec3> vertices;
	vertices.reserve(vcloud->getVertices().size());
	this->vertexFlagsAttribute.clear();
	this->vertexFlagsAttribute.reserve(vcloud->getVertices().size());
	this->copyModelBounds(vcloud);
	modelToCameraSpace = MatrixTransformationUtils::generateModelToCameraSpaceMatrix(
							 vcloud->getBounds());
	RVertexFlagAttribute flag;
	flag.disableAll();
	flag.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
	std::vector<vis::LWVertex*> &modelVertices = vcloud->getVertices();
	for( std::vector<vis::LWVertex*>::size_type i = 0; i < modelVertices.size(); i++ ) {
		vertices.push_back(modelVertices[i]->getCoords());
		vertexFlagsAttribute.push_back(flag);
	}
	nVertices = vertices.size();
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vertices);
	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
	originalModel = vcloud;
}

void RModel::loadRModelData(LightWeightPolygonMesh* mesh){
	this->freeRAMFromVideoCardBuffer();
	modelType = vis::CONSTANTS::LIGHT_WEIGHT_POLYGON_MESH;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<int> polygonsPolyhedronsIds;
	std::vector<int> vertexIds;
	vertices.reserve(mesh->getVertices().size());//min
	normals.reserve(mesh->getVertices().size());
	vertexIds.reserve(mesh->getVertices().size());
	this->vertexFlagsAttribute.clear();
	this->vertexFlagsAttribute.reserve(mesh->getVertices().size());
	this->copyModelBounds(mesh);
	modelToCameraSpace = MatrixTransformationUtils::generateModelToCameraSpaceMatrix(mesh->getBounds());
	std::vector<vis::LWPolygon*> &modelPolygons = mesh->getPolygons();
	for( std::vector<vis::LWPolygon*>::size_type i = 0; i < modelPolygons.size(); i++ )
		PolygonUtils::getTriangleVertices(modelPolygons[i],vertices,
										  normals,this->vertexFlagsAttribute,
										  polygonsPolyhedronsIds,
										  vertexIds);
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vertices);
	polygonPolyhedronIdsBufferObject = ShaderUtils::createDataBuffer<int>(polygonsPolyhedronsIds);
	vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(normals);
	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
	nVertices = vertices.size();
	for(int i = 0;i<nVertices;i++)
		polygonsPolyhedronsIds[i]=i;
	rmodelVertexPositionBufferObject = ShaderUtils::createDataBuffer<int>(polygonsPolyhedronsIds);
	vertexIdsBufferObject = ShaderUtils::createDataBuffer<int>(vertexIds);
	originalModel = mesh;
}

void RModel::loadRModelData(LightWeightPolyhedronMesh* mesh){
	loadRModelData((LightWeightPolygonMesh*)mesh);
}

void RModel::refreshSelectedElements(){
	ShaderUtils::setDataBuffer<RVertexFlagAttribute>(vertexFlagsDataBufferObject,vertexFlagsAttribute);
}

void RModel::freeRAMFromVideoCardBuffer(){
	glDeleteBuffers(1,&this->vertexFlagsDataBufferObject);
	glDeleteBuffers(1,&this->positionDataBufferObject);
	glDeleteBuffers(1,&this->vertexNormalDataBufferObject);
    glDeleteBuffers(1,&this->vertexScalarDataBufferObject);
	glDeleteBuffers(1,&rmodelVertexPositionBufferObject);
	glDeleteBuffers(1,&polygonPolyhedronIdsBufferObject);
    glDeleteBuffers(1,&tetrahedronVertexIdsBufferObject);
    glDeleteBuffers(1,&edgeVertexPositionsDataBufferObject);
    glDeleteBuffers(1,&edgeColorDataBufferObject);
	//glDeleteBuffers(1, &this->colorDataBufferObject);
	this->positionDataBufferObject = RModel::NULL_BUFFER;
	this->vertexFlagsDataBufferObject = RModel::NULL_BUFFER;
	this->vertexNormalDataBufferObject = RModel::NULL_BUFFER;
	rmodelVertexPositionBufferObject = RModel::NULL_BUFFER;
	polygonPolyhedronIdsBufferObject = RModel::NULL_BUFFER;
    vertexScalarDataBufferObject = RModel::NULL_BUFFER;
    tetrahedronVertexIdsBufferObject = RModel::NULL_BUFFER;
    edgeVertexPositionsDataBufferObject = RModel::NULL_BUFFER;
    edgeColorDataBufferObject = RModel::NULL_BUFFER;

}
glm::mat4 RModel::getMV(){
	if(recalculateMV){
		glm::mat4 model =  MatrixTransformationUtils::generateScaleMatrix(scale)*
						   MatrixTransformationUtils::generateTranslationMatrix(translation)*
						   MatrixTransformationUtils::generateRotationMatrix(rotation)*
						   getModelToCameraSpaceMatrix();
		//MV = viewMatrix*model;
		MV = model;
		recalculateMV = false;
		recalculateMVP = true;
		recalculateMVPVP = true;
	}
	return MV;
}
glm::mat4 RModel::getMVP(){
	if(recalculateMVP){
		MVP = getProjectionMatrix()*getMV();
		//MVP = getMV();
		recalculateMVP = false;
		recalculateMVPVP = true;
	}
	return MVP;
}
glm::mat4 RModel::getMVPVP(){
	if(recalculateMVPVP){
		MVPVP = getViewPortMatrix()*getMVP();
		recalculateMVPVP = false;
	}
	return MVPVP;
}

void RModel::setRotation(glm::vec3 v){
	rotation = v;
	recalculateMV = true;
	recalculateMVP = true;
	recalculateMVPVP = true;
	recalculateNormal = true;
}

void RModel::setTranslation(glm::vec3 v){
	translation = v;
	recalculateMV = true;
	recalculateMVP = true;
	recalculateMVPVP = true;
}
void RModel::setUseOrthoProjection(bool b){
	useOrthoProjection = b;
	recalculateMVP = true;
	recalculateMVPVP = true;
	recalculateModelCameraSpaceScale = true;
}

void RModel::setScale(float s){
	scale = s;
	recalculateModelCameraSpaceScale = true;
	recalculateMV = true;
	recalculateMVP = true;
	recalculateMVPVP = true;
}

void RModel::setViewPortSize(glm::vec2 rec, glm::vec2 z){
	viewPortSize = rec;
	viewPortZ = z;
	viewportMatrix = MatrixTransformationUtils::generateViewPortMatrix(0,0,rec.x,rec.y,z.x,z.y);
	float ratio = ((float)rec.x)/((float)rec.y);
	if(ratio>1.0f)
		orthoProjectionMatrix = MatrixTransformationUtils::generateOrthoProjectionMatrix(-0.5*ratio,
																						 0.5*ratio,
																						 0.5,-0.5,
																						 z.y, z.x);
	else
		orthoProjectionMatrix = MatrixTransformationUtils::generateOrthoProjectionMatrix(-0.5,0.5,0.5*1.0f/ratio,-0.5*1.0f/ratio,z.y, z.x);

	setPerspectiveValue(perspectiveValue);
	recalculateMVP = true;
	recalculateMVPVP = true;
}

glm::mat4 RModel::getViewPortMatrix(){
	return viewportMatrix;
}

glm::mat4 RModel::getOrthoProjectionMatrix(){
	return orthoProjectionMatrix;
}
glm::mat4 RModel::getProjectionMatrix(){
	if(useOrthoProjection)
		return getOrthoProjectionMatrix();
	return getPerspectiveProjectionMatrix();
}
void RModel::setPerspectiveValue(float newVal){
	perspectiveValue = newVal;
	float ratio = ((float)viewPortSize.x)/((float)viewPortSize.y);
	float frustum = MatrixTransformationUtils::CalcFrustumScale(perspectiveValue);
	float frustumRatio = MatrixTransformationUtils::CalcFrustumScale(perspectiveValue*ratio);
	perspectiveProjectionMatrix = MatrixTransformationUtils::generatePerspectiveProjectionMatrix(frustumRatio,frustum,
																								 viewPortZ.y, viewPortZ.x,
																								 0.5,-0.5,
																								 0.5*ratio,-0.5*ratio);
}

glm::mat4 RModel::getPerspectiveProjectionMatrix(){
	return perspectiveProjectionMatrix;
}

glm::mat4 RModel::getModelToCameraSpaceMatrix(){
	return modelToCameraSpace;
}
void RModel::setViewMatrix(glm::mat4 m){
	viewMatrix = m;
}

glm::vec2 RModel::getViewPortSize(){
	return viewPortSize;
}
glm::vec3& RModel::getCameraPosition(){
	return cameraPosition;
}
bool RModel::isOrthoProjectionInUse(){
	return useOrthoProjection;
}

glm::mat3 RModel::getNormalMatrix(){
	if(recalculateNormal){
		normalMatrix = glm::mat3(MatrixTransformationUtils::generateRotationMatrix(rotation));
		recalculateNormal = false;
	}
	return normalMatrix;
}
glm::mat4 RModel::getModelToCameraSpaceAndScaleMatrix(){
	if(recalculateModelCameraSpaceScale){
		recalculateModelCameraSpaceScale = false;
		modelToCameraScaleSpace = MatrixTransformationUtils::generateScaleMatrix(scale)*
								  getModelToCameraSpaceMatrix();//*/
		//modelToCameraScaleSpace = getMVP();
		//modelToCameraScaleSpace[3] = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	}
	return modelToCameraScaleSpace;
}

void RModel::setBackgroundColor(glm::vec4 s){
	backgroundColor = s;
}

glm::vec4 RModel::getBackgroundColor(){
	return backgroundColor;
}

int RModel::getModelType(){
	return modelType;
}
