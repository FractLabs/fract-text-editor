#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

int main() {
    std::vector<std::string> buffer;
    std::string text;
    std::string currentFile;
    std::cout << "***** Fract Text Editor *****\n";
    std::cout << "Type /save to save your texts.\n";
    std::cout << "Type /erase to erase your texts.\n";
    std::cout << "Type /open to open a file to edit.\n\n";

    while(true){
        std::cout << "> ";
        std::getline(std::cin,text);

        if(text=="/save"){
           if(currentFile.empty()){
                std::cout << "Enter the file name: \n";
                std::getline(std::cin, currentFile);
                std::filesystem::path p(currentFile);
                if(!p.has_extension()) p += ".txt";
           }
           std::ofstream file(currentFile);
           std::cout << "Saving to '" << currentFile <<"'...\n";
           if(!file){
                std::cerr << "Failed while saving file.\n";
                continue;
           }
           for(const auto& line : buffer){
                file << line << "\n";
           }
           file.close();
           std::cout << "Saved!\n";
        } else if(text=="/erase") {
            if(currentFile.empty()){
                std::cout << "No file currently open. Use /save or /open first.\n";
                continue;                
            }
            buffer.clear();
            std::ofstream file(currentFile, std::ios::trunc);
            std::cout << "Erasing content of '" << currentFile << "'...\n";
            file.close();
            std::cout << "Erased!\n";
        } else if(text=="/open"){
            std::string openPath;
            std::cout << "Enter the full path of the file to open: \n";
            std::getline(std::cin, openPath);

            if(std::filesystem::exists(openPath)){
                std::ifstream file(openPath);
                if(!file){
                    std::cerr << "Failed to open file. \n";
                    continue;
                }
                buffer.clear();
                std::string line;
                std::cout << "File content:\n";
                while(std::getline(file,line)){
                    std::cout << line << "\n";
                    buffer.push_back(line);
                }
                file.close();
                currentFile = openPath;
                std::cout << "File loaded into editor.\n";
            } else {
                std::cerr << "File does not exist.\n";
            }
        } else {
            buffer.push_back(text);
            text = "";
        }
    }

    return 0;
}