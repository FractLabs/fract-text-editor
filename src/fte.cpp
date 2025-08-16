#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <wx-3.2/wx/wx.h>
#include <wx-3.2/wx/filedlg.h>
#include <wx-3.2/wx/textctrl.h>

class FractTextEditor : public wxApp {
    wxFrame* frame;
    wxTextCtrl* textArea;
    public:
        bool OnInit() override {
            frame = new wxFrame(nullptr, wxID_ANY, "Fract Text Editor", wxDefaultPosition, wxSize(500, 350));
            wxPanel* panel = new wxPanel(frame);
            wxButton* New = new wxButton(panel, wxID_ANY, "New", wxPoint(5, 5), wxSize(100, 35));
            wxButton* Open = new wxButton(panel, wxID_ANY, "Open", wxPoint(115, 5), wxSize(100, 35));

            textArea = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(5, 50), wxSize(570,300), wxTE_MULTILINE | wxTE_RICH2); 

            Open->Bind(wxEVT_BUTTON, &FractTextEditor::OpenFile, this);
            frame->Show();
            return true;
        }

        void OpenFile(wxCommandEvent& event) {
            wxFileDialog openFileDialog(frame, _("Select a file"), "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (openFileDialog.ShowModal() == wxID_OK) {
                wxString path = openFileDialog.GetPath();

                wxMessageBox("Selected file:\n" + path);
            }
        }
};

wxIMPLEMENT_APP(FractTextEditor);

/* int main() {
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
} */