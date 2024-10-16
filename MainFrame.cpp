#include "MainFrame.h"
#include "Book.h"
#include <wx/textdlg.h>
#include <wx/listctrl.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)                       // Event handler for Exit
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)                     // Event handler for About
    EVT_MENU(ID_AddBook, MainFrame::OnAddBook)                   // Event handler for adding a book
    EVT_MENU(ID_RemoveBook, MainFrame::OnRemoveBook)             // Event handler for removing a book
    EVT_MENU(ID_UpdateBook, MainFrame::OnUpdateBook)             // Event handler for updating a book
    EVT_MENU(ID_CheckOutBook, MainFrame::OnCheckOutBook)         // Event handler for checking out a book
    EVT_MENU(ID_CheckInBook, MainFrame::OnCheckInBook)           // Event handler for checking in a book
    EVT_MENU(ID_Theme_Dark, MainFrame::OnThemeChange)            // Event handler for Dark Mode
    EVT_MENU(ID_Theme_Green, MainFrame::OnThemeChange)           // Event handler for Green Mode
    EVT_MENU(ID_Theme_Blue, MainFrame::OnThemeChange)            // Event handler for Blue Mode
    EVT_TEXT(ID_SearchCtrl, MainFrame::OnSearch)                 // Event handler for search input
    EVT_LIST_ITEM_SELECTED(wxID_ANY, MainFrame::OnBookSelected)  // Capture list item selection
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
    // Create menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_AddBook, "&Add Book...\tCtrl-A", "Add a new book");
    menuFile->Append(ID_UpdateBook, "&Update Book...\tCtrl-U", "Update an existing book");
    menuFile->Append(ID_RemoveBook, "&Remove Book...\tCtrl-R", "Remove an existing book");
    menuFile->Append(ID_CheckOutBook, "&Check Out Book...\tCtrl-O", "Check out a book");
    menuFile->Append(ID_CheckInBook, "&Check In Book...\tCtrl-I", "Check in a book");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenu* menuThemes = new wxMenu;
    menuThemes->Append(ID_Theme_Dark, "&Dark Mode", "Switch to Dark Mode");
    menuThemes->Append(ID_Theme_Green, "&Green Mode", "Switch to Green Mode");
    menuThemes->Append(ID_Theme_Blue, "&Blue Mode", "Switch to Blue Mode");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuThemes, "&Themes");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to the Library Management System!");

    // Create search control at the top
    searchCtrl = new wxTextCtrl(this, ID_SearchCtrl, "", wxPoint(10, 10), wxSize(580, 25));
    searchCtrl->SetHint("Search by Title, Author, or ISBN");

    // Create wxListCtrl to display the list of books
    bookListCtrl = new wxListCtrl(this, wxID_ANY, wxPoint(10, 50), wxSize(580, 300), wxLC_REPORT | wxBORDER_SUNKEN);
    bookListCtrl->InsertColumn(0, "Title", wxLIST_FORMAT_LEFT, 150);
    bookListCtrl->InsertColumn(1, "Author", wxLIST_FORMAT_LEFT, 120);
    bookListCtrl->InsertColumn(2, "ISBN", wxLIST_FORMAT_LEFT, 100);
    bookListCtrl->InsertColumn(3, "Available Copies", wxLIST_FORMAT_LEFT, 100);

    wxButton* checkOutButton = new wxButton(this, ID_CheckOutBook, "Check Out Book", wxPoint(220, 270), wxSize(150, 30));
    wxButton* checkInButton = new wxButton(this, ID_CheckInBook, "Check In Book", wxPoint(220, 310), wxSize(150, 30));

    // Bind event handlers to the buttons
    checkOutButton->Bind(wxEVT_BUTTON, &MainFrame::OnCheckOutBook, this);
    checkInButton->Bind(wxEVT_BUTTON, &MainFrame::OnCheckInBook, this);
    bookListCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::OnBookSelected, this);
}

void MainFrame::OnCheckOutBook(wxCommandEvent& event)
{
    long selected = bookListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == -1)
    {
        wxMessageBox("Please select a book to check out", "Error", wxOK | wxICON_WARNING);
        return;
    }

    wxString selectedISBN = bookListCtrl->GetItemText(selected, 2);  // Get the ISBN of the selected book
    Book* selectedBook = library.SearchBookByISBN(std::string(selectedISBN.mb_str()));

    if (selectedBook != nullptr)
    {
        if (selectedBook->GetAvailableCopies() > 0) // Check if there are available copies
        {
            selectedBook->CheckOut();  // Decrease available copies
            UpdateBookList();          // Update the list
            wxMessageBox("Book checked out successfully!", "Success", wxOK | wxICON_INFORMATION);
        }
        else
        {
            // Display message if there are no available copies
            wxMessageBox("No copies available for this book!", "Error", wxOK | wxICON_ERROR);
        }
    }
    else
    {
        wxMessageBox("Book not found!", "Error", wxOK | wxICON_ERROR);
    }
}


// Event handler for checking in a book
void MainFrame::OnCheckInBook(wxCommandEvent& event)
{
    long selected = bookListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == -1)
    {
        wxMessageBox("Please select a book to check in", "Error", wxOK | wxICON_WARNING);
        return;
    }

    wxString selectedISBN = bookListCtrl->GetItemText(selected, 2);  // Get the ISBN of the selected book
    Book* selectedBook = library.SearchBookByISBN(std::string(selectedISBN.mb_str()));

    if (selectedBook != nullptr)
    {
        selectedBook->CheckIn();  // Increase available copies
        UpdateBookList();
    }
}

// Method to update the book list after Check In/Out operations
void MainFrame::UpdateBookList(const wxString& filter)
{
    bookListCtrl->DeleteAllItems();  // Clear the existing items
    int index = 0;

    for (const auto& book : library.GetBooks())
    {
        wxString title = wxString::FromUTF8(book.GetTitle().c_str());
        wxString author = wxString::FromUTF8(book.GetAuthor().c_str());
        wxString isbn = wxString::FromUTF8(book.GetISBN().c_str());
        wxString copies = wxString::Format("%d", book.GetAvailableCopies());

        if (filter.IsEmpty() || title.Lower().Contains(filter.Lower()) || author.Lower().Contains(filter.Lower()) || isbn.Lower().Contains(filter.Lower()))
        {
            long itemIndex = bookListCtrl->InsertItem(index, title);
            bookListCtrl->SetItem(itemIndex, 1, author);
            bookListCtrl->SetItem(itemIndex, 2, isbn);
            bookListCtrl->SetItem(itemIndex, 3, copies);
            index++;
        }
    }
}

void MainFrame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a simple Library Management System.", "About LMS", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnAddBook(wxCommandEvent& event)
{
    // Get the book title
    wxTextEntryDialog dialogTitle(this, "Enter the book title", "Add Book");
    if (dialogTitle.ShowModal() == wxID_OK)
    {
        wxString title = dialogTitle.GetValue();
        if (title.IsEmpty()) // Check if title is empty
        {
            wxMessageBox("Title cannot be empty!", "Error", wxOK | wxICON_ERROR);
            return; // Exit the function if title is empty
        }

        // Get the author name
        wxTextEntryDialog dialogAuthor(this, "Enter the book author", "Add Book");
        if (dialogAuthor.ShowModal() == wxID_OK)
        {
            wxString author = dialogAuthor.GetValue();
            if (author.IsEmpty()) // Check if author is empty
            {
                wxMessageBox("Author cannot be empty!", "Error", wxOK | wxICON_ERROR);
                return; // Exit the function if author is empty
            }

            // Get the ISBN (only digits allowed)
            wxTextEntryDialog dialogISBN(this, "Enter the book ISBN (Digits only)", "Add Book");
            if (dialogISBN.ShowModal() == wxID_OK)
            {
                wxString isbn = dialogISBN.GetValue();

                // Check if ISBN contains only digits
                if (!isbn.IsNumber())  // wxString::IsNumber checks if the string contains only digits
                {
                    wxMessageBox("ISBN must be numeric (digits only)!", "Error", wxOK | wxICON_ERROR);
                    return; // Exit the function if ISBN contains non-digit characters
                }

                // Check for duplicate ISBN
                Book* existingBook = library.SearchBookByISBN(std::string(isbn.mb_str()));
                if (existingBook != nullptr)
                {
                    wxMessageBox("A book with this ISBN already exists!", "Error", wxOK | wxICON_ERROR);
                    return;  // Exit the function to prevent adding a duplicate book
                }

                // Get the number of available copies
                wxTextEntryDialog dialogCopies(this, "Enter number of available copies", "Add Book");
                if (dialogCopies.ShowModal() == wxID_OK)
                {
                    wxString copiesStr = dialogCopies.GetValue();
                    long copies;
                    if (!copiesStr.ToLong(&copies) || copies <= 0) // Check if copies is a valid number
                    {
                        wxMessageBox("Number of copies must be a valid numeric value greater than 0!", "Error", wxOK | wxICON_ERROR);
                        return; // Exit the function if invalid
                    }

                    // If all fields are valid, create and add the new book
                    Book newBook(std::string(title.mb_str()), std::string(author.mb_str()), std::string(isbn.mb_str()), copies);
                    library.AddBook(newBook);

                    UpdateBookList();
                    wxMessageBox("Book added successfully!", "Success", wxOK | wxICON_INFORMATION);
                }
            }
        }
    }
}





void MainFrame::OnUpdateBook(wxCommandEvent& event)
{
    wxTextEntryDialog dialogISBN(this, "Enter the ISBN of the book to update", "Update Book");
    if (dialogISBN.ShowModal() == wxID_OK)
    {
        wxString isbn = dialogISBN.GetValue();

        Book* bookToUpdate = library.SearchBookByISBN(std::string(isbn.mb_str()));
        if (bookToUpdate)
        {
            wxTextEntryDialog dialogTitle(this, "Enter the new title", "Update Book", wxString(bookToUpdate->GetTitle()));
            if (dialogTitle.ShowModal() == wxID_OK)
            {
                wxString newTitle = dialogTitle.GetValue();

                wxTextEntryDialog dialogAuthor(this, "Enter the new author", "Update Book", wxString(bookToUpdate->GetAuthor()));
                if (dialogAuthor.ShowModal() == wxID_OK)
                {
                    wxString newAuthor = dialogAuthor.GetValue();

                    wxTextEntryDialog dialogCopies(this, "Enter the number of available copies", "Update Book", wxString::Format("%d", bookToUpdate->GetAvailableCopies()));
                    if (dialogCopies.ShowModal() == wxID_OK)
                    {
                        wxString copiesStr = dialogCopies.GetValue();
                        long newCopies;
                        copiesStr.ToLong(&newCopies);

                        library.UpdateBook(std::string(bookToUpdate->GetISBN()), std::string(newTitle.mb_str()), std::string(newAuthor.mb_str()), newCopies);
                        UpdateBookList();
                        wxMessageBox("Book updated successfully!", "Success", wxOK | wxICON_INFORMATION);
                    }
                }
            }
        }
        else
        {
            wxMessageBox("Book not found!", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnRemoveBook(wxCommandEvent& event)
{
    wxTextEntryDialog dialogISBN(this, "Enter the ISBN of the book to remove", "Remove Book");

    if (dialogISBN.ShowModal() == wxID_OK)
    {
        wxString isbn = dialogISBN.GetValue();
        Book* bookToRemove = library.SearchBookByISBN(std::string(isbn.mb_str()));

        if (bookToRemove != nullptr)
        {
            library.RemoveBook(std::string(isbn.mb_str()));
            UpdateBookList();
            wxMessageBox("Book removed successfully!", "Success", wxOK | wxICON_INFORMATION);
        }
        else
        {
            wxMessageBox("Book not found!", "Error", wxOK | wxICON_ERROR);
        }
    }
}

// Handle theme change
void MainFrame::OnThemeChange(wxCommandEvent& event)
{
    int theme = event.GetId();
    ApplyTheme(theme);
}

// Apply the selected theme
void MainFrame::ApplyTheme(int theme)
{
    wxColour bgColour, textColour;

    if (theme == ID_Theme_Dark)
    {
        bgColour = wxColour(45, 45, 48);  // Dark background
        textColour = wxColour(255, 255, 255);  // Light text
    }
    else if (theme == ID_Theme_Green)
    {
        bgColour = wxColour(34, 139, 34);  // Green background
        textColour = wxColour(255, 255, 255);  // Light text
    }
    else if (theme == ID_Theme_Blue)
    {
        bgColour = wxColour(70, 130, 180);  // Blue background
        textColour = wxColour(255, 255, 255);  // Light text
    }

    SetBackgroundColour(bgColour);
    bookListCtrl->SetBackgroundColour(bgColour);
    bookListCtrl->SetForegroundColour(textColour);
    searchCtrl->SetBackgroundColour(bgColour);
    searchCtrl->SetForegroundColour(textColour);

    Refresh();
}

// Handle search input
void MainFrame::OnSearch(wxCommandEvent& event)
{
    wxString searchQuery = searchCtrl->GetValue();
    UpdateBookList(searchQuery);
}

void MainFrame::OnBookSelected(wxListEvent& event)
{
    long selectedItemIndex = event.GetIndex();
    wxString selectedISBN = bookListCtrl->GetItemText(selectedItemIndex, 2);
}
