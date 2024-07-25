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

void addToCart(int productId, int quantity) {
    Product* current = inventory;
    while (current != nullptr) {
        if (current->id == productId) {
            if (current->quantity >= quantity) {
                cart[cartItemCount] = {productId, quantity};
                cartItemCount++;
                current->quantity -= quantity;
                cout << "Added " << quantity << " of " << current->name << " to cart." << endl;
            } else {
                cout << "Not enough stock for " << current->name << "." << endl;
            }
            return;
        }
        current = current->next;
    }
    cout << "Product ID not found." << endl;
}

void generateReceipt() {
    double total = 0;
    cout << "\nReceipt:" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << "Quantity\tItem\t\tPrice\t\t\tTotal" << endl;
    cout << "---------------------------------------------------------------------" << endl;
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
    cout << "=====================================================================" << endl;
    cout << "Total:\t\t\t\t\t\t\tPHP " << fixed << setprecision(2) << total << endl;
    cout << "=====================================================================" << endl;
}

void checkout() {
    double total = 0;
    for (int i = 0; i < cartItemCount; ++i) {
        Product* current = inventory;
        while (current != nullptr) {
            if (current->id == cart[i].productId) {
                total += cart[i].quantity * current->price;
                break;
            }
            current = current->next;
        }
    }

    if (currentUser->wallet >= total) {
        char confirm;
        cout << "Your total is PHP " << fixed << setprecision(2) << total << ". Do you want to proceed with the checkout? (y/n): ";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            currentUser->wallet -= total;
            generateReceipt();
            cartItemCount = 0;  // Empty the cart after checkout
            cout << "Checkout successful. Thank you for your purchase!" << endl;
        } else {
            cout << "Checkout cancelled." << endl;
        }
    } else {
        cout << "Insufficient funds. Please top-up your wallet." << endl;
    }
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        cout << "User limit reached. Cannot register more users." << endl;
        return;
    }
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    if (username == "admin") {
        cout << "Username 'admin' is not allowed." << endl;
        return;
    }
    cout << "Enter password: ";
    cin >> password;
    users[userCount] = {username, password, 0.0};
    userCount++;
    cout << "User registered successfully." << endl;
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

void modifyProduct() {
    if (!isAdmin) {
        cout << "Only admins can modify products." << endl;
        return;
    }
    int id, quantity;
    string name;
    double price;
    cout << "Enter product ID to modify: ";
    cin >> id;
    Product* current = inventory;
    while (current != nullptr) {
        if (current->id == id) {
            cout << "Enter new product name: ";
            cin.ignore();
            getline(cin, name);

            // Check for duplicate product name
            Product* temp = inventory;
            while (temp != nullptr) {
                if (temp->name == name && temp->id != id) {
                    cout << "Product name already exists. Please enter a unique name." << endl;
                    return;
                }
                temp = temp->next;
            }

            cout << "Enter new product quantity: ";
            cin >> quantity;
            if (quantity < 0) {
                cout << "Quantity cannot be negative." << endl;
                return;
            }

            cout << "Enter new product price: ";
            cin >> price;
            if (price < 0) {
                cout << "Price cannot be negative." << endl;
                return;
            }

            current->name = name;
            current->quantity = quantity;
            current->price = price;
            cout << "Product modified successfully." << endl;
            return;
        }
        current = current->next;
    }
    cout << "Product ID not found." << endl;
}

void removeProduct() {
    if (!isAdmin) {
        cout << "Only admins can remove products." << endl;
        return;
    }
    int id;
    cout << "Enter product ID to remove: ";
    cin >> id;

    Product* current = inventory;
    Product* prev = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (prev == nullptr) {
                inventory = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            cout << "Product removed successfully." << endl;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Product ID not found." << endl;
}

void cleanupInventory() {
    Product* current = inventory;
    while (current != nullptr) {
        Product* next = current->next;
        delete current;
        current = next;
    }
    inventory = nullptr;
}

void userMenu() {
    int choice;
    string input;
    while (true) {
        cout << "\n|========================|\n";
        cout << "|  Supermarket Checkout  |\n";
        cout << "|========================|\n";
        cout << "| 1. Register            |\n";
        cout << "| 2. Login               |\n";
        cout << "| 3. Exit                |\n";
        cout << "|========================|";
        cout << "\nChoose an option: ";
        cin >> input;

        if (!isNumber(input)){
            cout << "Invalid Input. Enter a number." << endl;
            continue;
        }

        choice = stoi(input);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    if (isAdmin) {
                        return;  // Exit the user menu and proceed to the admin menu
                    } else {
                        return;  // Exit the user menu and proceed to the shopping menu
                    }
                }
                break;
            case 3:
                cout << "Exiting. Thank you!" << endl;
                cleanupInventory();
                exit(0);
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void shoppingMenu() {
    int choice, productId, quantity;
    string input;
    while (true) {
        cout << "\n|=========================|\n";
        cout << "|1. View Inventory        |\n";
        cout << "|2. View Cart             |\n";
        cout << "|3. Add to cart           |\n";
        cout << "|4. Check Out             |\n";
        cout << "|5. Top-up Wallet         |\n";
        cout << "|6. Check Balance         |\n";
        cout << "|7. Logout                |\n";
        cout << "|=========================|\n" ;
        cout << "Choose an option: ";
        cin >> input;

        if (!isNumber(input)){
            cout << "Invalid Input. Enter a number." << endl;
            continue;
        }

        choice = stoi(input);

        switch (choice) {
            case 1:
                displayInventory();
                break;
            case 2:
                displayCart();
                break;
            case 3:
                displayInventory();
                cout << "Enter product ID to add to cart: ";
                cin >> productId;
                cout << "Enter quantity: ";
                cin >> quantity;
                addToCart(productId, quantity);
                break;
            case 4:
                checkout();
                break;
            case 5:
                topUpWallet();
                break;
            case 6:
                checkBalance();
                break;
            case 7:
                currentUser = nullptr;
                cout << "Logged out successfully." << endl;
                return;  // Exit the shopping menu and return to the user menu
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void adminMenu() {
    string input;
    int choice;
    while (true) {
        cout << "\n|=========================|\n";
        cout << "|1. Add Product           |\n";
        cout << "|2. Modify Product        |\n";
        cout << "|3. Remove Product        |\n";
        cout << "|4. View Inventory        |\n";
        cout << "|5. Logout                |\n";
        cout << "|=========================|\n" ;
         cout << "Choose an option: ";
        cin >> input;

        if (!isNumber(input)){
            cout << "Invalid Input. Enter a number." << endl;
            continue;
        }

        choice = stoi(input);
        
        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                modifyProduct();
                break;
            case 3:
                removeProduct();
                break;
            case 4:
                displayInventory();
                break;
            case 5:
                isAdmin = false;
                cout << "Logged out successfully." << endl;
                return;  // Exit the admin menu and return to the user menu
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

int main() {
    initializeInventory();
    while (true) {
        if (currentUser == nullptr && !isAdmin) {
            userMenu();
        } else if (isAdmin) {
            adminMenu();
        } else {
            shoppingMenu();
        }
    }
}



