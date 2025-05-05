#include <bits/stdc++.h>
using namespace std;

enum BookStatus { AVAILABLE, BORROWED };

class IBook {
public:
    int id;
    string title, author, publisher;
    virtual void display() = 0;
    virtual ~IBook() = default;
};

class Novel : public IBook {
public:
    Novel(int id, string title, string author, string publisher) {
        this->id = id;
        this->title = title;
        this->author = author;
        this->publisher = publisher;
    }
    void display() override {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\nPublisher: " << publisher << endl;
    }
};

class ResearchPaper : public IBook {
public:
    string conference, year;
    ResearchPaper(int id, string title, string author, string publisher, string conference, string year) {
        this->id = id;
        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->conference = conference;
        this->year = year;
    }
    void display() override {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\nPublisher: " << publisher
             << "\nConference: " << conference << "\nYear: " << year << endl;
    }
};

class IRate {
public:
    virtual int getPrice() = 0;
    virtual ~IRate() = default;
};

class InRate : public IRate {
    int price;
public:
    InRate(int p) : price(p) {}
    int getPrice() override { return price; }
};

class OutRate : public IRate {
    int price;
public:
    OutRate(int p) : price(p) {}
    int getPrice() override { return price; }
};

class BookItem {
    shared_ptr<IBook> book;
    BookStatus status = AVAILABLE;
    shared_ptr<IRate> rate;

public:
    BookItem(shared_ptr<IBook> book) : book(book) {}

    int rentOutBook(int days) {
        if (status == AVAILABLE) {
            status = BORROWED;
            rate = make_shared<OutRate>(10);
            return rate->getPrice() * days;
        }
        cout << "Book is already borrowed!" << endl;
        return 0;
    }

    int rentInBook(int hours) {
        if (status == BORROWED) {
            status = AVAILABLE;
            rate = make_shared<InRate>(5);
            return rate->getPrice() * hours;
        }
        cout << "Book is already available!" << endl;
        return 0;
    }

    int getPrice() const {
        return rate ? rate->getPrice() : 0;
    }

    int getBookId() const {
        return book->id;
    }

    shared_ptr<IBook> getBook() const {
        return book;
    }

    BookStatus getStatus() const {
        return status;
    }
};

class BookManager {
    map<int, shared_ptr<BookItem>> availableBooks;
    map<int, shared_ptr<BookItem>> borrowedBooks;

public:
    void addBook(shared_ptr<BookItem> item) {
        availableBooks[item->getBookId()] = item;
    }

    void borrowBook(shared_ptr<BookItem> item, int days) {
        int id = item->getBookId();
        if (availableBooks.count(id)) {
            int price = item->rentOutBook(days);
            borrowedBooks[id] = item;
            availableBooks.erase(id);
            cout << "Book borrowed! Price: " << price << endl;
        } else {
            cout << "Book not available!" << endl;
        }
    }

    void returnBook(shared_ptr<BookItem> item, int hours) {
        int id = item->getBookId();
        if (borrowedBooks.count(id)) {
            int price = item->rentInBook(hours);
            availableBooks[id] = item;
            borrowedBooks.erase(id);
            cout << "Book returned! Cost: " << price << endl;
        } else {
            cout << "Book not borrowed!" << endl;
        }
    }

    void displayAvailableBooks() const {
        cout << "Available Books:\n";
        for (const auto& it : availableBooks) {
            it.second->getBook()->display();
            cout << endl;
        }
    }
};

class UserManager {
    map<int, shared_ptr<BookItem>> borrowedBooks;

public:
    void borrow(shared_ptr<BookItem> item) {
        borrowedBooks[item->getBookId()] = item;
    }

    void returnBook(shared_ptr<BookItem> item) {
        borrowedBooks.erase(item->getBookId());
    }

    void displayBorrowedBooks() const {
        cout << "Borrowed Books:\n";
        for (const auto& it : borrowedBooks) {
            it.second->getBook()->display();
            cout << endl;
        }
    }
};

class Library {
    map<int, shared_ptr<BookItem>> allBooks;
    BookManager bookManager;
    UserManager userManager;

public:
    void addBook(shared_ptr<BookItem> item) {
        allBooks[item->getBookId()] = item;
        bookManager.addBook(item);
    }

    void borrowBook(int bookId, int days) {
        if (allBooks.count(bookId)) {
            auto item = allBooks[bookId];
            bookManager.borrowBook(item, days);
            userManager.borrow(item);
        }
    }

    void returnBook(int bookId, int hours) {
        if (allBooks.count(bookId)) {
            auto item = allBooks[bookId];
            bookManager.returnBook(item, hours);
            userManager.returnBook(item);
        }
    }

    void showAvailableBooks() const {
        bookManager.displayAvailableBooks();
    }

    void showUserBooks() const {
        userManager.displayBorrowedBooks();
    }
};
int main() {
    Library library;

    library.addBook(make_shared<BookItem>(make_shared<Novel>(1, "The Great Gatsby", "F. Scott Fitzgerald", "Scribner")));
    library.addBook(make_shared<BookItem>(make_shared<ResearchPaper>(2, "AI in Healthcare", "John Doe", "IEEE", "ICML", "2023")));

    library.showAvailableBooks();

    library.borrowBook(1, 5);

 
    library.showUserBooks();


    library.returnBook(1, 2);

    library.showAvailableBooks();

    return 0;
}