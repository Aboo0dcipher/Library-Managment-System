#include <iostream>
#include <string>
#include <limits>

using namespace std;



class Book {
private:
    string title, author, category;
    int ISBN, pages;
    int availableUnits; 

public:
    Book() : ISBN(0), availableUnits(0) {}

    void setBookDetails();
    void displayBook() const;
    bool isAvailable() const { return availableUnits > 0; }
    void borrowBook();
    void returnBook();
    int getISBN() const { return ISBN; } 
};

// Node for BookList to user linklist
class Node {   
    Book book;
    Node* next;
public:
    Node(Book b) {
        book=b;
        next=NULL;
    }   
     //Getter function
    Book getbook(){
        return book;
    }
    friend class BookList;
};

class BookList{
private:
    Node* listptr;//points to the first node

public:
    BookList() {
        listptr=NULL;
    }
    void addBook(Book b);
    void removeBook(int ISB);
    Node* searchBook(int ISBN);   
    void displayAllBooks() const;

};

// User Class
class User {
protected:
    string name, ID;
    int lateFees; 


public:
    User(string Name, string id) { 
        name=Name;
        ID=id; 
    }
    //virual function to dspalay the user
    virtual void displayUser() const {
        cout << "Name: " << name << ", ID: " << ID<<endl;
    }

    //Getter method for getID
    string getID() const {
         return ID;}
};

// Admin Class
class UserNode {
public:
    User *user;
    UserNode *next;

    UserNode(User* u) {
        user=u;
        next=NULL;
    }
   
};


class Admin : public User {
private:
    UserNode* head;  // Head of the linklist of the users

public: 
    Admin() : User("Admin", "001"), head(nullptr) {}

    void registerUser(const string name, const string id);
    bool validateUserLogin(const string id) const;
    void viewAllUsers() const;
    void deleteUser(const string id);

            
                ~Admin() {
                    while (head != nullptr) {
                        UserNode* temp = head;
                        head = head->next;
                        delete temp->user;
                        delete temp;
                    }
                }


};


void Admin::registerUser(const string name, const string id){
        UserNode *newNode = new UserNode(new User(name, id)); 
        newNode->next = head;
        head = newNode;
        cout << "User '" << name << "' registered successfully.\n";
    }



bool Admin::validateUserLogin(const string id) const {
        UserNode* curr = head;
        //using the follwoing will look for the id throughout the linklist if found then i will allow the user to register otherwise will return invalid user 
        while (curr != nullptr) {
            if (curr->user->getID()== id) {
                return true;  
            }
            curr = curr->next;
        }
        return false;  // User not found
}



void Admin:: viewAllUsers() const {
        cout << "--- Registered Users ---\n";
        UserNode* curr = head;

        if (!curr) {
            cout << "No registered users.\n";
            return;
        }

        while (curr != nullptr) {
            curr->user->displayUser();
            curr = curr->next;
        }
}



void Admin::deleteUser(const string id) {
        UserNode* curr = head;
        UserNode* prev = nullptr;

        while (curr != nullptr) {
            if (curr->user->getID() == id) {
                if (prev == nullptr) {
                    head = curr->next;  // Deleting the head node
                } else {
                    prev->next = curr->next;  // Bypass the current node
                }
                cout << "User with ID " << id << " deleted successfully.\n";
                // delete curr->user;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "User with ID " << id << " not found.\n";
}

class DateBase {
protected:
    int day, month, year;

public:
    virtual void inputDate() = 0;      
    virtual void displayDate() const = 0; 
};


class Date : public DateBase {
public:
    void inputDate() override {
        cout << "Enter day (DD): "; cin >> day;
        cout << "Enter month (MM): "; cin >> month;
        cout << "Enter year (YYYY): "; cin >> year;
    }

    void displayDate() const override {
        cout << day << "/" << month << "/" << year << endl;
    }

    //Getter methods
    int getyear() const {
        return year;
    }

    int getmonth() const {
        return month;
    }

    int getday() const {
        return day;
    }
};




// Transaction Class
class Transaction {
private:
    int ISBN;
    Date issueDate, dueDate, returnDate;
    int lateFees; 

public:
    Transaction() : ISBN(0), lateFees(0) {}
    void borrowBook(Book book, const Date issue, const Date due);
    void returnBook(Book book, const Date returnDate);


};


void Transaction::borrowBook(Book book, const Date issue, const Date due) {
        if (book.isAvailable()) {
            book.borrowBook();
            ISBN = book.getISBN();
            issueDate = issue;
            dueDate = due;
            cout << "\nTransaction recorded: Book borrowed successfully!\n";
        } else {
            cout << "This book is not available for borrowing.\n";
        }
}
    

void Transaction::returnBook(Book book, const Date returnDate) {
    book.returnBook();
    this->returnDate = returnDate; //removing naming conflict

    if (returnDate.getyear() > dueDate.getyear() ||
        (returnDate.getyear() == dueDate.getyear() && returnDate.getmonth() > dueDate.getmonth()) ||
        (returnDate.getyear() == dueDate.getyear() && returnDate.getmonth() == dueDate.getmonth() && returnDate.getday() > dueDate.getday())) {
        
        int lateDays = 0;
        
        int totalDaysDue = dueDate.getyear() * 365 + dueDate.getmonth() * 30 + dueDate.getday();
        int totalDaysReturned = returnDate.getyear() * 365 + returnDate.getmonth() * 30 + returnDate.getday();
        
        lateDays = totalDaysReturned - totalDaysDue ;

        if (lateDays > 0) {
            lateFees += lateDays * 10 ; // Assuming 10 Ruppes  per day as late fee
            cout << "Book returned late. Late Fees accumulated: " << lateFees <<" Rupees"<< endl;
            
        }
    }

    cout << "Transaction recorded: Book returned successfully!\n"<<endl;
}






// Function to get a valid integer input
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        // Check for invalid input
        if (cin.fail()) {
            cin.clear();  // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "Invalid input! Please enter a number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
            return value;
        }
    }
}






// Book Class Functions
void Book::setBookDetails() {
    cout << "\n--- Enter Book Details ---\n";
    cout << "Enter Book Title: "; cin >> title;
    cout << "Enter Author Name: "; cin >> author;
    cout << "Enter Book Category: "; cin >> category;
    cout << "Enter ISBN (integer): "; cin >> ISBN;
    cout << "Enter Number of Pages (integer): "; cin >> pages;
    cout << "Enter Available Units: "; cin >> availableUnits;
}

void Book::displayBook() const {
    if (isAvailable()) {
        cout<<"\nBooks Available:";
        cout << "Title: " << title << ", Author: " << author
             << ", ISBN: " << ISBN << ", Pages: " << pages
             << ", Available Units: " << availableUnits << endl;
    }
}

void Book::borrowBook() {
    if (availableUnits > 0) {
        availableUnits--;
        cout << "One copy of the book borrowed successfully!\n";
    } else {
        cout << "No copies available for this book.\n";
    }
}

void Book::returnBook() {
    availableUnits++;
    cout << "One copy of the book returned successfully!\n";
}





// BookList Class Functions
void BookList::addBook(Book b) {
    Node* newnode = new Node(b);
    newnode->next=NULL;
    if (listptr == NULL) {
        listptr = newnode;
    } else {
        Node* temp = listptr;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newnode;
    }
    cout << "\nBook added successfully!\n";
}

Node* BookList::searchBook(int ISBN) {
    Node* temp = listptr;
    while (temp != NULL) {
        if (temp->book.getISBN()== ISBN) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void BookList::removeBook(int ISBN){
    if (listptr == NULL) {
        cout << "\nNo books to remove.\n";
        return;
    }

    Node* temp = listptr;
    Node* prev = NULL;
    
    //if book is root=listptr;
    if (temp != NULL && temp->book.getISBN()== ISBN) {
        listptr = temp->next;
        delete(temp);
        cout << "\nBook removed successfully!"<<endl;
        return;
    }

    while (temp != NULL && temp->book.getISBN()!= ISBN) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "\nBook not found.\n";
        return;
    }

    prev->next = temp->next; //set the node to NULL if the node is in the last node or link to b/t nodes if node is in the middel
    delete (temp);
    cout << "\nBook removed successfully!\n";
}


void BookList::displayAllBooks() const {
    if (listptr==NULL) {
        cout << "No books available.\n";
        return;
    }
    Node* temp = listptr;
    while (temp) {
        if (temp->book.isAvailable()) temp->book.displayBook();
        temp = temp->next;
    }
}
int main() {
    Admin admin;
    BookList bookList;
    int choice;

    while (true) {
        cout << "\n--- Library Management System ---\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as User\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInt("");

        switch (choice) {
            case 1: { // Admin Login
            while(true){
                string adminID;
                cout<<"Enter Admin ID: ";
                cin>>adminID;
                if(adminID=="001"){
                    cout<<"Admin Login Successful. Welcome!"<<endl;
                    break;
                }
                else{
                    cout<<"Invalid Login ID. Please try again!"<<endl;
                }
            }
                while (true) {
                    cout << "\n--- Admin Menu ---\n";
                    cout << "1. Register User\n";
                    cout << "2. Delete User\n";
                    cout << "3. View All Users\n";
                    cout << "4. Add Book\n";
                    cout << "5. Logout\n";
                    cout << "Enter your choice: ";
                    int adminChoice = getValidInt("");

                    switch (adminChoice) {
                        case 1: {
                            string name, id;
                            cout << "Enter User Name: ";
                            cin >> name;
                            cout << "Enter User ID: ";
                            cin >> id;
                            admin.registerUser(name, id);
                            break;
                        }
                        case 2: {
                            string id;
                            cout << "Enter User ID to delete: ";
                            cin >> id;
                            admin.deleteUser(id);
                            break;
                        }
                        case 3:
                            admin.viewAllUsers();
                            break;
                        case 4:
                        {
                            Book book;
                                book.setBookDetails(); 
                                bookList.addBook(book);
                        }
                         
                            break;
                        case 5:
                            cout << "Logging out...\n";
                            goto endAdminMenu;
                        default:
                            cout << "Invalid choice! Please try again.\n";
                    }
                }
                endAdminMenu:
                break;
            }
            case 2: { // User Login
                string userID;
                cout << "Enter User ID to login: ";
                cin >> userID;

                if (admin.validateUserLogin(userID)) {
                    cout << "Login successful!\n";
                    while (true) {
                        cout << "\n--- User Menu ---\n";
                        cout << "1. Search Book\n";
                        cout << "2. View All Books\n";
                        cout << "3. Borrow Book\n";
                        cout << "4. Return Book\n";
                        cout << "5. Logout\n";
                        cout << "Enter your choice: ";
                        int userChoice = getValidInt("");

                        switch (userChoice) {
                           case 1:{
                                int ISBN;
                                cout << "Enter ISBN to search: ";
                                ISBN = getValidInt("");
                                
                                Node* bookNode = bookList.searchBook(ISBN); //if found return object which is temp
                                if (bookNode) {
                                    bookNode->getbook().displayBook();  
                                } else {
                                    cout << "Book not found.\n";
                                }
                                break;
                            }

                            case 2:
                                bookList.displayAllBooks();
                                break;

                           case 3: {
                                int ISBN;
                                Date issueDate, dueDate;
                                cout << "Enter ISBN of the book to borrow: ";
                                ISBN = getValidInt("");
                                Node* bookNode = bookList.searchBook(ISBN);  // Get Node*
                                if (bookNode && bookNode->getbook().isAvailable()) {
                                    issueDate.inputDate();
                                    cout << "Enter due date:\n";
                                    dueDate.inputDate();
                                    Transaction transaction;
                                    transaction.borrowBook(bookNode->getbook(), issueDate, dueDate);  // Pass the book object
                                } else {
                                    cout << "Book not available or not found.\n";
                                }
                                break;
                            }

                       case 4: {
                                int ISBN;
                                Date returnDate;
                                cout << "Enter ISBN of the book to return: ";
                                ISBN = getValidInt("");
                                Node* bookNode = bookList.searchBook(ISBN);  // Get Node*
                                if (bookNode) {
                                    returnDate.inputDate();
                                    Transaction transaction;
                                    transaction.returnBook(bookNode->getbook(), returnDate);  // Pass the book object
                                } else {
                                    cout << "Book not found.\n";
                                }
                                break;
                            }

                            case 5:
                                cout << "Logging out...\n";
                                goto endUserMenu;
                            default:
                                cout << "Invalid choice! Please try again.\n";
                        }
                    }
                    endUserMenu:
                    break;
                } else {
                    cout << "Invalid User ID! Login failed.\n";
                }
                break;
            }
            case 3:
                cout << "Exiting the system...\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}
