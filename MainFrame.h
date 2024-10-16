#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include "Library.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    wxStaticBitmap* bookCoverImage;                     // New member to display book covers
    wxListCtrl* bookListCtrl;
    wxTextCtrl* searchCtrl;                             // Search control for user input

    Library library;

    void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
    void OnCheckOutBook(wxCommandEvent& event);
    void OnCheckInBook(wxCommandEvent& event);
    void OnAddBook(wxCommandEvent& event);
    void OnUpdateBook(wxCommandEvent& event);
    void OnRemoveBook(wxCommandEvent& event);
    void OnThemeChange(wxCommandEvent& event);
    void ApplyTheme(int theme);
    void UpdateBookList(const wxString& filter = "");
    void OnSearch(wxCommandEvent& event);
    void OnBookSelected(wxListEvent& event);

    wxDECLARE_EVENT_TABLE();
};

// Define IDs for menu items
enum
{
    ID_AddBook = 1,                                      // ID for Add Book
    ID_Theme_Dark = 2,                                   // ID for Dark Mode
    ID_Theme_Green = 3,                                  // ID for Green Mode
    ID_Theme_Blue = 4,                                   // ID for Blue Mode
    ID_SearchCtrl = 5,                                   // ID for Search Control
    ID_UpdateBook = 6,                                   // ID for Update Book
    ID_RemoveBook = 7,                                   // ID for Remove Book
    ID_CheckOutBook = 8,                                 // ID for Check Out
    ID_CheckInBook = 9                                   // ID for Check In
};

#endif // MAINFRAME_H
