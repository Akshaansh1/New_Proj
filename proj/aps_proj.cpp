#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip> // For setw


using namespace std;

const int prime = 101;

vector<int> searchRabinKarp(const string &text, const string &pattern) {
    vector<int> occurrences;
    int m = pattern.length();
    int n = text.length();
    int patternHash = 0;
    int textHash = 0;

    for (int i = 0; i < m; ++i) {
        patternHash = (patternHash + pattern[i]) % prime;
        textHash = (textHash + text[i]) % prime;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == textHash) {
            int j;
            for (j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j])
                    break;
            }
            if (j == m)
                occurrences.push_back(i);
        }
        if (i < n - m) {
            textHash = (textHash - text[i] + text[i + m]) % prime;
            if (textHash < 0)
                textHash += prime;
        }
    }
    return occurrences;
}

void searchInventory(const string &item) {
    ifstream inventoryFile("inventory.txt");
    if (!inventoryFile.is_open()) {
        cout << "Error: Unable to open inventory file." << endl;
        return;
    }

    string line;
    vector<pair<string, string>> foundItems; // Vector to store found items

    while (getline(inventoryFile, line)) {
        vector<int> occurrences = searchRabinKarp(line, item);
        if (!occurrences.empty()) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string particulars = line.substr(0, pos);
                string quantity = line.substr(pos + 1);
                foundItems.push_back(make_pair(particulars, quantity));
            }
        }
    }

    if (!foundItems.empty()) {
        cout << "\033[36m"; // Cyan color
        cout << "+----------------------+----------+" << endl;
        cout << "| Item Particulars    | Quantity  |" << endl;
        cout << "+----------------------+----------+" << endl;
        for (const auto &item : foundItems) {
            cout << "| " << setw(20) << left << item.first << " | " << setw(8) << right << item.second << " |" << endl;
        }
        cout << "+----------------------+----------+" << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
    cout << "\033[0m"; // Reset color
}


const string inventoryFileName = "inventory.txt";

struct Item {
    string particulars;
    string quantity;
};

// Function to read inventory from file
vector<Item> readInventory() {
    vector<Item> inventory;
    ifstream inFile(inventoryFileName);
    if (!inFile.is_open()) {
        cout << "Error: Unable to open inventory file." << endl;
        return inventory;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string particulars, quantity;
        getline(ss, particulars, ',');
        getline(ss, quantity);
        inventory.push_back({particulars, quantity});
    }

    inFile.close();
    return inventory;
}

// Function to write inventory to file
void writeInventory(const vector<Item>& inventory) {
    ofstream outFile(inventoryFileName);
    if (!outFile.is_open()) {
        cout << "Error: Unable to open inventory file for writing." << endl;
        return;
    }

    for (const auto& item : inventory) {
        outFile << item.particulars << "," << item.quantity << endl;
    }

    outFile.close();
}

// Function to add item to inventory
void addItemToInventory(const string& particulars, const string& quantity) {
    vector<Item> inventory = readInventory();
    inventory.push_back({particulars, quantity});
    writeInventory(inventory);
    cout << "Item added to inventory." << endl;
}

// Function to delete item from inventory
void deleteItemFromInventory(const string& particulars) {
    vector<Item> inventory = readInventory();
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.particulars == particulars;
    });
    if (it != inventory.end()) {
        inventory.erase(it);
        writeInventory(inventory);
        cout << "Item deleted from inventory." << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
}

// Function to upgrade item quantity in inventory
void upgradeItemInInventory(const string& particulars, const string& newQuantity) {
    vector<Item> inventory = readInventory();
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.particulars == particulars;
    });
    if (it != inventory.end()) {
        it->quantity = newQuantity;
        writeInventory(inventory);
        cout << "Item upgraded in inventory." << endl;
    } else {
        cout << "Item not found in inventory." << endl;
    }
}

// Function to manage inventory
void manageInventory() {
    cout << "\033[32m"<<endl;
    cout << "1. Add Item" << endl;
    cout << "\033[31m";
    cout << "2. Delete Item" << endl;
    cout << "\033[33m";
    cout << "3. Upgrade Item" << endl;
    cout << "\033[0m";
    cout << "Enter option: ";
    int option;
    cin >> option;
    cin.ignore();

    switch (option) {
        case 1:
            {
                string particulars, quantity;
                cout << "Enter item particulars: ";
                getline(cin, particulars);
                cout << "Enter item quantity: ";
                getline(cin, quantity);
                addItemToInventory(particulars, quantity);
                break;
            }
        case 2:
            {
                string particulars;
                cout << "Enter item particulars to delete: ";
                getline(cin, particulars);
                deleteItemFromInventory(particulars);
                break;
            }
        case 3:
            {
                string particulars, newQuantity;
                cout << "Enter item particulars to upgrade: ";
                getline(cin, particulars);
                cout << "Enter new quantity: ";
                getline(cin, newQuantity);
                upgradeItemInInventory(particulars, newQuantity);
                break;
            }
        default:
            cout << "Invalid option." << endl;
    }
}



int main() {
    cout << "\n\n\n";
    cout << "\033[32m";
    cout << "\t\t\t MAIN MENU" << endl;
    cout << "\033[0m";
    cout << "------------------------------------------------------------------------" << endl;
    cout << "1. \t\t\tMANAGE INVENTORY\t[M]\t" << endl;     // Cpp OOPS
    cout << "2. \t\t\tPROCESS ORDERS\t\t[P]\t" << endl;     //0/1 Knapsack
    cout << "3. \t\t\tOPTIMISE ROUTES\t\t[O]\t" << endl;    // Floydd
    cout << "4. \t\t\tSEARCH INVENTORY\t[S]\t" << endl;     //Rabin - Karp
    cout << "5. \t\t\tVIEW REPORTS\t\t[V]\t" << endl;
    cout << "\033[31m";
    cout << "\t\t\t    EXIT\t\t[E]\t" << endl;
    cout << "\033[0m";
    cout << "-------------------------------------------------------------------------" << endl;

    cout << "\033[32m";
    cout << "CHOOSE A SUITABLE OPTION" << endl;
    cout << "\033[0m";

    char ch;
    cin >> ch;

    string searchItem; 

    switch (ch) {

    case 'M':
        cin.ignore();
        manageInventory();
        break;

    case 'S':
        cin.ignore();
        cout << "Enter the item you want to search: ";
        getline(cin, searchItem);
        searchInventory(searchItem);
        break;

    default:
        cout << "\033[31m";
        cout << "Enter Right Option" << endl;
        // Clear input stream to prevent infinite loop
        cout << "\033[0m";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}
