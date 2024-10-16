#include "Library.h"

void Library::AddBook(const Book& book) 
{
    books.push_back(book);
}

void Library::RemoveBook(const std::string& isbn) 
{
    for (auto it = books.begin(); it != books.end(); ++it) 
    {
        if (it->GetISBN() == isbn) {
            books.erase(it);
            break;
        }
    }
}

Book* Library::SearchBookByISBN(const std::string& isbn) {
    for (auto& book : books) 
    {
        if (book.GetISBN() == isbn) 
        {
            return &book;
        }
    }
    return nullptr;
}

// New method: returns the list of books
const std::vector<Book>& Library::GetBooks() const 
{
    return books;
}

void Library::UpdateBook(const std::string& isbn, const std::string& newTitle, const std::string& newAuthor, int newCopies) 
{
    for (auto& book : books) 
    {
        if (book.GetISBN() == isbn) 
        {
            book = Book(newTitle, newAuthor, isbn, newCopies);  // Update the book details
            break;
        }
    }
}