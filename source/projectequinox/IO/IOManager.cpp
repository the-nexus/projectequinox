#include <fstream>
#include "GL/glew.h"
#include "IOManager.h"


bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer){
	std::ifstream file(filePath.c_str(), std::ios::binary);
	if (file.fail()){
		perror(filePath.c_str());
		return false;
	}

	//seek to the end
	file.seekg(0, std::ios::end);

	//Get the file size
	int fileSize = (int)file.tellg();
	file.seekg(0, std::ios::beg);

	//Reduce the file size by any header bytes
	fileSize -= (int)file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);

	file.close();

	return true;
}