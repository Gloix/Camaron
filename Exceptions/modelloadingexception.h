#ifndef MODELLOADINGEXCEPTION_H
#define MODELLOADINGEXCEPTION_H
#include "Exceptions/unknownextensionexception.h"
#include "exceptionmessage.h"
class ModelLoadingException:
		public UnknownExtensionException,
		public ExceptionMessage
{
	public:
		ModelLoadingException(std::string filepath,
							  int mt,
							  std::string message);
		virtual ~ModelLoadingException();
		int getModelType();
	protected:
		int modelType;
};

#endif // MODELLOADINGEXCEPTION_H
