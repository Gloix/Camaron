#include "fileutils.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

FileUtils::FileUtils(){}

std::string FileUtils::getFileNameExtension(std::string filename){
	std::string::size_type lastPeriod = filename.rfind( '.' );
	if( lastPeriod != std::string::npos && lastPeriod + 1 < filename.length() )
		return filename.substr( lastPeriod + 1 );
	return "";//no extension found
}

std::string FileUtils::getFileNameWithoutExtension(std::string filename){
	std::string::size_type lastPeriod = filename.rfind( '.' );
	if( lastPeriod != std::string::npos && lastPeriod + 1 < filename.length() )
		return filename.substr( 0,lastPeriod );
	return filename;//no extension found
}
std::string FileUtils::getFileNameWithoutPath(std::string filename){
	std::string::size_type lastSlash = filename.rfind( '/' );
	if( lastSlash != std::string::npos && lastSlash + 1 < filename.length() )
		return filename.substr( lastSlash + 1 );
	lastSlash = filename.rfind( '\\' );
	if( lastSlash != std::string::npos && lastSlash + 1 < filename.length() )
		return filename.substr( lastSlash + 1 );
	return filename;//no extension found
}
bool FileUtils::fileExists( std::string filename){
	FILE* file = fopen( filename.c_str(), "r" );
	if(file){
		fclose(file);
		return true;
	}
	return false;
}


char* FileUtils::getFileToBuffer(std::string filename, long* fileSize){
	if(FileUtils::fileExists(filename)){
		long fileSizeLocal = FileUtils::getFileSize(filename);
		char* fileBuffer = (char*)malloc(fileSizeLocal+1L);
		FILE* file = fopen( filename.c_str(), "r" );
		long readed = fread(fileBuffer,1,fileSizeLocal,file);
		fclose(file);
		fileBuffer[fileSizeLocal] = '\0';
		*fileSize = readed;
		return fileBuffer;
	}
	*fileSize = 0L;
	return (char*)0;
}

long FileUtils::getFileSize(std::string filename){
	struct stat st;
	stat(filename.c_str(), &st);
	return st.st_size;
}
std::string FileUtils::getFileToString(std::string filename){
	std::ifstream myfile (filename.c_str());
	if(!myfile.is_open()){
		return std::string();
	}
	std::string filedataCurrent( ( std::istreambuf_iterator<char> ( myfile ) ) ,
								 std::istreambuf_iterator<char> ( ) ) ;
	myfile.close( );
	return filedataCurrent;
}
void FileUtils::getFileToStringsByLine(std::string filename,std::vector<std::string>& lines){
	std::ifstream myfile (filename.c_str());
	if(!myfile.is_open()){
		return;
	}
	std::string line;
	while(std::getline(myfile, line))
		lines.push_back(line);
	myfile.close();
}
bool FileUtils::saveToFile(std::string filename, std::string content){
	std::ofstream file(filename.c_str(), std::ios_base::binary);
	file << content;
	file.close();
	return true;
}


std::istream& FileUtils::safeGetline(std::ifstream& is, std::string& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for(;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if(sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if(t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}
