#include "Book.h"

Book::Book(const std::string& title,
           const std::string& author,
           const std::string& isbn,
           int copies) : title(title),
                         author(author),
                         isbn(isbn),
                         availableCopies(copies) {}

std::string Book::GetTitle() const                  
{
    return title;
}

std::string Book::GetAuthor() const 
{
    return author;
}

std::string Book::GetISBN() const 
{
    return isbn;
}

int Book::GetAvailableCopies() const 
{
    return availableCopies;
}

void Book::SetCopies(int copies) 
{
    availableCopies = copies;
}
void Book::CheckOut()
{
    if (availableCopies > 0)
    {
        availableCopies--;  // Decrease copies when checked out
    }
}

void Book::CheckIn()
{
    availableCopies++;  // Increase copies when checked in
}