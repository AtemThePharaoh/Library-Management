#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book 
{
public:
    Book(const std::string& title,
         const std::string& author,
         const std::string& isbn,
         int copies);

    std::string GetTitle() const;
    std::string GetAuthor() const;
    std::string GetISBN() const;
    int GetAvailableCopies() const;

    void SetCopies(int copies);
    void CheckOut();  // Decrease available copies
    void CheckIn();   // Increase available copies

private:
    std::string title;
    std::string author;
    std::string isbn;
    int availableCopies;
};

#endif // BOOK_H