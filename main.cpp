#include "MainFrame.h"
#include <wx/wx.h>

class LibraryApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(LibraryApp);

bool LibraryApp::OnInit() {
    MainFrame* mainFrame = new MainFrame("Library Management System");
    mainFrame->Show(true);
    return true;
}