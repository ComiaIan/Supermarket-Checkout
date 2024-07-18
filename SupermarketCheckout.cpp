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
