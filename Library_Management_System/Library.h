#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include "Book.h"

class Library 
{
public:
    void AddBook(const Book& book);
    void RemoveBook(const std::string& isbn);
    Book* SearchBookByISBN(const std::string& isbn);
    const std::vector<Book>& GetBooks() const;  // Method to return all books
    void UpdateBook(const std::string& isbn, const std::string& newTitle, const std::string& newAuthor, int newCopies);

private:
    std::vector<Book> books;
};

#endif // LIBRARY_H