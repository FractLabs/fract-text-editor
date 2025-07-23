#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main() {
    std::vector<std::string> buffer;
    std::string text;
    std::string saveName;
    std::cout << "***** Fract Text Editor *****\n";
    std::cout << "Type /save to save your texts.\n";
    std::cout << "Type /erase to erase your texts.\n\n";

    while(true){
        std::cout << "> ";
        std::getline(std::cin,text);

        if(text=="/save"){
            std::cout << "Enter the file name: \n";
            std::getline(std::cin,saveName);
            saveName.append(".txt");

            std::ofstream file(saveName);

            std::cout << "Saving...\n";
            if(!file){
                std::cerr << "Failed while creating file.\n";
                continue;
            }
            for (const auto& line : buffer){
                file << line << "\n";
            }
            file.close();
            std::cout << "Saved!\n";
        } else if(text=="/erase") {
            std::ofstream file(saveName);
            std::cout << "Erasing...\n";
            file << "";
        } else {
            buffer.push_back(text);
            text = "";
        }
    }

    return 0;
}