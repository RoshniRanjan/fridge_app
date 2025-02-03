#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits> // For numeric_limits (used in future extensions or validations)

using namespace std;

// Product class represents an individual product stored in the refrigerator.
class Product {
private:
    string name; // The name of the product
    double quantity; // The quantity of the product in the refrigerator
    string expirationDate; // The expiration date of the product

public:
    // Default constructor to initialize a product with empty values
    Product() {
        this->name = "";
        this->quantity = 0.0;
        this->expirationDate = "";
    }

    // Parametrized constructor to initialize the product with specific values
    Product(const string &productName, double productQuantity, const string &productExpirationDate) {
        this->name = productName;
        this->quantity = productQuantity;
        this->expirationDate = productExpirationDate;
    }

    // Getter method to retrieve the product's name
    string getName() const {
        return name;
    }

    // Getter method to retrieve the product's quantity
    double getQuantity() const {
        return quantity;
    }

    // Getter method to retrieve the product's expiration date
    string getExpirationDate() const {
        return expirationDate;
    }

    // Method to add a specified quantity to the existing product
    void addQuantity(double additionalQuantity) {
        quantity += additionalQuantity;
    }

    // Method to consume (reduce) the quantity of the product by a specified amount
    void consumeQuantity(double consumedQuantity) {
        quantity -= consumedQuantity;
    }
};

// Refrigerator class manages multiple products, tracks actions performed, and handles refrigerator operations
class Refrigerator {
private:
    unordered_map<string, Product> products; // A map to store products, using product name as the key
    vector<string> history; // A log of all actions (insertions, consumptions) performed on the refrigerator

    // Private helper function to log actions performed on the refrigerator
    void logAction(const string &action) {
        history.push_back(action); // Store the action string in the history log
    }

    // Private helper function to check if a product is expired based on the current date
    bool isExpired(const string &currentDate, const string &expirationDate) {
        return currentDate >= expirationDate; // Compare dates (as strings) to determine expiration
    }

public:
    // Method to insert a new product into the refrigerator or update the quantity of an existing product
    void insertProduct(const string &productName, double productQuantity, const string &productExpirationDate) {
        // Validation: Ensure the quantity is greater than zero before inserting
        if (productQuantity <= 0) {
            cout << "Error: Product quantity must be greater than zero." << endl;
            return;
        }

        // If the product already exists, update its quantity. Otherwise, insert it as a new product
        if (products.find(productName) != products.end()) {
            products[productName].addQuantity(productQuantity); // Add the quantity to the existing product
        } else {
            Product newProduct(productName, productQuantity, productExpirationDate); // Create a new product
            products[productName] = newProduct; // Insert the new product into the map
        }

        // Log the action of inserting a product
        logAction("Inserted " + to_string(productQuantity) + " of " + productName);
    }

    // Method to consume (reduce) the quantity of a specific product
    void consumeProduct(const string &productName, double productQuantity) {
        // Validation: Ensure the consumed quantity is greater than zero
        if (productQuantity <= 0) {
            cout << "Error: Consumed quantity must be greater than zero." << endl;
            return;
        }

        // Check if the product exists in the refrigerator
        if (products.find(productName) == products.end()) {
            cout << "Product not found in refrigerator." << endl;
            return;
        }

        // Ensure there's enough quantity of the product to consume
        if (products[productName].getQuantity() < productQuantity) {
            cout << "Not enough quantity to consume." << endl;
            return;
        }

        // Consume the specified quantity and update the product
        products[productName].consumeQuantity(productQuantity);

        // Log the action of consuming a product
        logAction("Consumed " + to_string(productQuantity) + " of " + productName);

        // If the product quantity reaches zero, remove it from the refrigerator
        if (products[productName].getQuantity() == 0) {
            products.erase(productName);
        }
    }

    // Method to display the current status of the refrigerator (list all products with quantities and expiration dates)
    void showStatus() {
        cout << "\n--- Current Refrigerator Status ---" << endl;
        if (products.empty()) {
            cout << "The refrigerator is empty." << endl;
            return;
        }

        // Iterate through the products and display their information
        for (const auto &item : products) {
            cout << "- " << item.second.getName() << ": " << item.second.getQuantity()
                 << " (Expires: " << item.second.getExpirationDate() << ")" << endl;
        }
    }

    // Method to display the history of actions performed on the refrigerator
    void showHistory() {
        cout << "\n--- History of Actions ---" << endl;
        if (history.empty()) {
            cout << "No actions recorded yet." << endl;
            return;
        }

        // Iterate through the history and print each action
        for (const string &entry : history) {
            cout << "- " << entry << endl;
        }
    }

    // Method to check and remove expired products based on the current date
    void checkExpirations(const string &currentDate) {
        cout << "\n--- Checking Expired Products ---" << endl;
        bool expiredFound = false;
        // Iterate through the products and check if any are expired
        for (auto it = products.begin(); it != products.end();) {
            if (isExpired(currentDate, it->second.getExpirationDate())) {
                cout << "Product " << it->second.getName() << " has expired. Please remove it." << endl;
                it = products.erase(it); // Remove expired product from the refrigerator
                expiredFound = true;
            } else {
                ++it;
            }
        }

        if (!expiredFound) {
            cout << "No expired products found." << endl;
        }
    }

    // Method to generate a shopping list based on consumed products
    void generateShoppingList() {
        unordered_map<string, double> consumptionMap;
        // Parse the history to track consumed products and their quantities
        for (const string &entry : history) {
            if (entry.find("Consumed") != string::npos) {
                size_t quantityStart = entry.find(" ") + 1;
                size_t quantityEnd = entry.find(" of ");
                double consumedQuantity = stod(entry.substr(quantityStart, quantityEnd - quantityStart));
                string productName = entry.substr(quantityEnd + 4);
                consumptionMap[productName] += consumedQuantity; // Aggregate consumed quantities
            }
        }

        cout << "\n--- Generated Shopping List ---" << endl;
        if (consumptionMap.empty()) {
            cout << "No items to suggest for shopping." << endl;
            return;
        }

        // Display the shopping list with suggested quantities to buy
        for (const auto &item : consumptionMap) {
            cout << "- Buy more " << item.first << " (" << item.second << ")" << endl;
        }
    }
};

// Function to display the main menu for refrigerator management actions
void showMenu() {
    cout << "\n*** Refrigerator Menu ***" << endl;
    cout << "1. Insert Product" << endl;
    cout << "2. Consume Product" << endl;
    cout << "3. Show Refrigerator Status" << endl;
    cout << "4. Show Action History" << endl;
    cout << "5. Check Expired Products" << endl;
    cout << "6. Generate Shopping List" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Refrigerator fridge; // Create an instance of the Refrigerator class
    int choice; // User's menu choice
    string productName, expirationDate, currentDate; // Product-related details
    double productQuantity; // Quantity of the product

    cout << "WELCOME!" << endl;
    cout<<"Refrigerator PathLock 2025!"<<endl;
    cout << "Note: Use the date format YYYY-MM-DD for expiration dates.\n" << endl;

    while (true) {
        showMenu(); // Display the menu
        cin >> choice; // Get user's choice from the menu

        // Perform actions based on user's choice
        switch (choice) {
        case 1:
            cout << "Enter product name: ";
            cin >> productName;
            cout << "Enter product quantity: ";
            cin >> productQuantity;
            cout << "Enter expiration date (YYYY-MM-DD): ";
            cin >> expirationDate;
            fridge.insertProduct(productName, productQuantity, expirationDate);
            break;

        case 2:
            cout << "Enter product name: ";
            cin >> productName;
            cout << "Enter quantity to consume: ";
            cin >> productQuantity;
            fridge.consumeProduct(productName, productQuantity);
            break;

        case 3:
            fridge.showStatus(); // Show the current refrigerator status
            break;

        case 4:
            fridge.showHistory(); // Display the history of actions
            break;

        case 5:
            cout << "Enter current date (YYYY-MM-DD): ";
            cin >> currentDate;
            fridge.checkExpirations(currentDate); // Check for expired products
            break;

        case 6:
            fridge.generateShoppingList(); // Generate shopping list
            break;

        case 7:
            cout << "Exiting program. Goodbye!" << endl; // Exit the program
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl; // Handle invalid menu choices
            break;
        }
    }

    return 0;
}
