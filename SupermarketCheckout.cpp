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

void initializeInventory() {
    inventory = new Product{1, "Apple", 50, 100, nullptr};
    inventory->next = new Product{2, "Banana", 60, 150, nullptr};
    inventory->next->next = new Product{3, "Orange", 40, 80, nullptr};
    inventory->next->next->next = new Product{4, "Milk", 90, 50, nullptr};
    inventory->next->next->next->next = new Product{5, "Bread", 70, 60, nullptr};
}

bool isNumber(const string& str) {
    for (char const &c : str) {
        if (isdigit(c) ==  0)
        return false;
    }
    return true;
}

void displayInventory() {
    cout << "Inventory:" << endl;
    Product* current = inventory;
    while (current != nullptr) {
        cout << "ID: " << current->id 
             << ", Name: " << current->name 
             << ", Price: PHP " << fixed << setprecision(2) << current->price 
             << ", Quantity: " << current->quantity << endl;
        current = current->next;
    }
}

void displayCart() {
    if (cartItemCount == 0) {
        cout << "Your cart is empty." << endl;
        return;
    }
    double total = 0;
    cout << "Your Cart:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Quantity\tItem\t\tPrice\t\tTotal" << endl;
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < cartItemCount; ++i) {
        Product* current = inventory;
        while (current != nullptr) {
            if (current->id == cart[i].productId) {
                double cost = cart[i].quantity * current->price;
                total += cost;
                cout << cart[i].quantity << "\t\t" << current->name 
                     << "\t\tPHP " << fixed << setprecision(2) << current->price 
                     << "\t\tPHP " << fixed << setprecision(2) << cost << endl;
                break;
            }
            current = current->next;
        }
    }
    cout << "----------------------------------------" << endl;
    cout << "Total:\t\t\t\t\tPHP " << fixed << setprecision(2) << total << endl;
    cout << "----------------------------------------" << endl;
}

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
