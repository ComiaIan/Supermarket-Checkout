#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

const int MAX_CART_ITEMS = 10;
const int MAX_USERS = 10;

typedef struct Product {
    int id;
    string name;
    double price;
    int quantity;
    struct Product* next;
} Product;

typedef struct {
    int productId;
    int quantity;
} CartItem;

typedef struct {
    string username;
    string password;
    double wallet;
} User;

Product* inventory = nullptr;
CartItem cart[MAX_CART_ITEMS];
User users[MAX_USERS];
int cartItemCount = 0;
int userCount = 0;
User* currentUser = nullptr;
bool isAdmin = false;

bool loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    if (username == "admin" && password == "admin") {
        isAdmin = true;
        cout << "Admin login successful. Welcome, admin!" << endl;
        return true;
    }
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username && users[i].password == password) {
            currentUser = &users[i];
            isAdmin = false;
            cout << "Login successful. Welcome, " << username << "!" << endl;
            return true;
        }
    }
    cout << "Invalid username or password." << endl;
    return false;
}

void topUpWallet() {
    if (currentUser == nullptr) {
        cout << "Please login first." << endl;
        return;
    }
    double amount;
    cout << "Enter amount to top-up: ";
    cin >> amount;
    if (amount < 0) {
        cout << "Amount cannot be negative." << endl;
        return;
    }
    currentUser->wallet += amount;
    cout << "Wallet topped-up successfully. Current balance: PHP " << fixed << setprecision(2) << currentUser->wallet << endl;
}

void checkBalance() {
    if (currentUser == nullptr) {
        cout << "Please login first." << endl;
        return;
    }
    cout << "Current balance: PHP " << fixed << setprecision(2) << currentUser->wallet << endl;
}

void addProduct() {
    if (!isAdmin) {
        cout << "Only admins can add products." << endl;
        return;
    }
    int id, quantity;
    string name;
    double price;
    cout << "Enter product ID: ";
    cin >> id;

    // Check for duplicate product ID
    Product* current = inventory;
    while (current != nullptr) {
        if (current->id == id) {
            cout << "Product ID already exists. Please enter a unique ID." << endl;
            return;
        }
        current = current->next;
    }

    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);

    // Check for duplicate product name
    current = inventory;
    while (current != nullptr) {
        if (current->name == name) {
            cout << "Product name already exists. Please enter a unique name." << endl;
            return;
        }
        current = current->next;
    }

    cout << "Enter product quantity: ";
    cin >> quantity;
    if (quantity < 0) {
        cout << "Quantity cannot be negative." << endl;
        return;
    }

    cout << "Enter product price: ";
    cin >> price;
    if (price < 0) {
        cout << "Price cannot be negative." << endl;
        return;
    }

    Product* newProduct = new Product{id, name, price, quantity, inventory};
    inventory = newProduct;
    cout << "Product added successfully." << endl;
}