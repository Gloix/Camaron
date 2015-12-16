#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <string>
#include <vector>

struct AcceptedFileFormat{
	public:
		AcceptedFileFormat(std::string fname, std::string fext){
			fileFormatName = fname; fileFormatExt = fext;
		}
		std::string fileFormatName;
		std::string fileFormatExt;
};

class FileUtils
{
	public:
		static bool fileExists( std::string filename);
		static std::string getFileNameWithoutExtension(std::string filename);
		static std::string getFileNameExtension(std::string filename);
		static std::string getFileNameWithoutPath(std::string filename);
		static long getFileSize(std::string filename);
		static char* getFileToBuffer(std::string filename,long* fileSize);
		static std::string getFileToString(std::string filename);
		static bool saveToFile(std::string filename, std::string content);
		static void getFileToStringsByLine(std::string filename,
										  std::vector<std::string>& lines);
		static std::istream& safeGetline(std::ifstream& is, std::string& t);
	private:
		FileUtils();
};

#endif // FILEUTILS_H
