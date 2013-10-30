#include "modelloadingexception.h"

ModelLoadingException::ModelLoadingException(std::string filepath,
											 int mt,
											 std::string message):
	UnknownExtensionException(filepath),
	ExceptionMessage(message)
{
	modelType = mt;
}
ModelLoadingException::~ModelLoadingException(){

}

int ModelLoadingException::getModelType(){
	return modelType;
}
