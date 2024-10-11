#include "../include/Utils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string Utils::readFromFile(const char *path){
    std::string code;
    std::ifstream vertFile;
    try {
       	vertFile.open(path);
       	std::stringstream vertStream;
       	vertStream << vertFile.rdbuf();
       	code = vertStream.str();
    }catch (std::ifstream::failure e) {
        throw std::runtime_error("Failed to Read from the file: " + std::string(e.what()));
    }
    return code;
}
