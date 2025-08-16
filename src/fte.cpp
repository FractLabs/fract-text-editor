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
    wxString currentPath;
    public:
        bool OnInit() override {
            frame = new wxFrame(nullptr, wxID_ANY, "Fract Text Editor", wxDefaultPosition, wxSize(500, 350));
            wxPanel* panel = new wxPanel(frame);
            wxButton* New = new wxButton(panel, wxID_ANY, "New", wxPoint(5, 5), wxSize(100, 35));
            wxButton* Open = new wxButton(panel, wxID_ANY, "Open", wxPoint(110, 5), wxSize(100, 35));
            wxButton* Close = new wxButton(panel, wxID_ANY, "Close", wxPoint(215, 5), wxSize(100, 35));
            wxButton* Save = new wxButton(panel, wxID_ANY, "Save", wxPoint(320, 5), wxSize(100, 35));

            textArea = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(5, 50), wxSize(490,295), wxTE_MULTILINE | wxTE_RICH2); 
            textArea->Hide();
            
            New->Bind(wxEVT_BUTTON, &FractTextEditor::NewFile, this);
            Open->Bind(wxEVT_BUTTON, &FractTextEditor::OpenFile, this);
            Close->Bind(wxEVT_BUTTON, &FractTextEditor::CloseFile, this);
            Save->Bind(wxEVT_BUTTON, &FractTextEditor::SaveFile, this);

            frame->Show();
            return true;
        }

        void OpenFile(wxCommandEvent& event) {
            wxFileDialog openFileDialog(frame, _("Select a file"), "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (openFileDialog.ShowModal() == wxID_OK) {
                wxString path = openFileDialog.GetPath();
                std::ifstream file(path.ToStdString());

                if (!file.is_open()) {
                    wxMessageBox("Failed to open file!", "Error", wxICON_ERROR);
                    return;
                }

                std::string line, content;
                while (std::getline(file, line)) {
                    content += line + "\n";
                }
                file.close();

                textArea->SetValue(content);
                textArea->Show();
                frame->Layout();

                currentPath = path;
                frame->SetLabel("Fract Text Editor - " + path);
            }
        }

        void NewFile(wxCommandEvent& event) {
            textArea->Clear();
            textArea->Show();
            frame->Layout();

            currentPath.Clear();
            frame->SetLabel("Fract Text Editor - New File");
        }

        void SaveFile(wxCommandEvent& event) {
            if (!textArea->IsShown()) {
                wxMessageBox("No file is open to save!", "Warning", wxICON_WARNING);
                return;
            }

            if (currentPath.IsEmpty()) {
                wxFileDialog saveFileDialog(frame, _("Save file as"), "", "", "All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

                if (saveFileDialog.ShowModal() == wxID_OK) {
                    currentPath = saveFileDialog.GetPath();
                } else {
                    return;
                }
            }

            std::ofstream file(currentPath.ToStdString());
            if (!file.is_open()) {
                wxMessageBox("Failed to save file!", "Error", wxICON_ERROR);
                return;
            }

            file << textArea->GetValue().ToStdString();
            file.close();

            wxMessageBox("File saved sucessfully!", "Info", wxICON_INFORMATION);
            frame->SetLabel("Fract Text Editor - " + currentPath);
        }

        void CloseFile(wxCommandEvent& event) {
            if (textArea->IsShown()) {
                textArea->Hide();
                frame->Layout();
                frame->SetLabel("Fract Text Editor");
                currentPath.Clear();
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
