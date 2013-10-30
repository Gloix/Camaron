#ifndef UNKNOWNEXTENSIONEXCEPTION_H
#define UNKNOWNEXTENSIONEXCEPTION_H
#include <string>

class UnknownExtensionException
{
	public:
		UnknownExtensionException(std::string filepath);
		std::string getExtension();
		std::string getFilename();
	protected:
		std::string filename;
		std::string extension;
};

#endif // UNKNOWNEXTENSIONEXCEPTION_H
