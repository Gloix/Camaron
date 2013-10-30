#include "unknownextensionexception.h"
#include "Utils/fileutils.h"
UnknownExtensionException::UnknownExtensionException(std::string filepath):
	filename(FileUtils::getFileNameWithoutPath(filepath)),
	extension(FileUtils::getFileNameExtension(filepath))
{
}

std::string UnknownExtensionException::getExtension(){
	return extension;
}

std::string UnknownExtensionException::getFilename(){
	return filename;
}
