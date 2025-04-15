#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct User {
    string username;
    string password;
    double balance;
};
//make the txt file to save data
vector<User> users;
string dataFile = "users.txt";
//save users and their info
void loadUsers() {
    users.clear();
    ifstream file(dataFile);
    string userName, password;
    double bal;
    while (file >> userName >> password >> bal) {
        users.push_back({ userName, password, bal });
    }
    file.close();
}

void saveUsers() {
    ofstream file(dataFile);
    for (const auto& user : users) {
        file << user.username << " " << user.password << " " << user.balance << endl;
    }
    file.close();
}

// create accounts
void registerUser() {
    string userName, password;
    cout << "Enter username: ";
    cin >> userName;
    cout << "Enter password: ";
    cin >> password;

    for (const auto& u : users) {
        if (u.username == userName) {
            cout << "User already exists.\n";
            return;
        }
    }

    users.push_back({ userName, password, 0.0 });
    saveUsers();
    cout << "User registered successfully!\n";
}

// check users
int loginUser(bool& isAdmin) {
    string userName, password;
    cout << "Enter username: ";
    cin >> userName;
    cout << "Enter password: ";
    cin >> password;

    if (userName == "admin" && password == "admin123") {
        isAdmin = true;
        cout << "Parent login successful!\n";
        return -1;
    }

    for (int i = 0; i < users.size(); ++i) {
        if (users[i].username == userName && users[i].password == password) {
            cout << "Login successful!\n";
            return i;
        }
    }

    cout << "Invalid credentials.\n";
    return -1;
}
void adminMenu() {
    int choice;
    do {
        cout << "\n--- Parent Portal ---\n";
        cout << "1. View All Users\n2. Search User\n3. Delete User\n4. Add Funds\n5. Reset Password\n6. System Summary\n7. Logout\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n--- All Users ---\n";
            for (const auto& u : users) {
                cout << "Username: " << u.username << ", Balance: $" << u.balance << endl;
            }
        }
        else if (choice == 2) {
            string userName;
            cout << "Enter username to search: ";
            cin >> userName;
            bool found = false;
            for (const auto& u : users) {
                if (u.username == userName) {
                    cout << "Found: " << u.username << ", Balance: $" << u.balance << endl;
                    found = true;
                    break;
                }
            }
            if (!found) cout << "User not found.\n";
        }
        else if (choice == 3) {
            string userName;
            cout << "Enter username to delete: ";
            cin >> userName;
            for (int i = 0; i < users.size(); ++i) {
                if (users[i].username == userName) {
                    users.erase(users.begin() + i);
                    saveUsers();
                    cout << "User deleted.\n";
                    break;
                }
            }
        }
        else if (choice == 4) {
            string userName;
            double amount;
            cout << "Enter username to add funds to: ";
            cin >> userName;
            cout << "Enter amount: ";
            cin >> amount;

            for (auto& u : users) {
                if (u.username == userName) {
                    u.balance += amount;
                    saveUsers();
                    cout << "Funds added.\n";
                    break;
                }
            }
        }
        else if (choice == 5) {
            string userName, newPassword;
            cout << "Enter username to reset password: ";
            cin >> userName;
            cout << "Enter new password: ";
            cin >> newPassword;

            for (auto& u : users) {
                if (u.username == userName) {
                    u.password = newPassword;
                    saveUsers();
                    cout << "Password reset.\n";
                    break;
                }
            }
        }
        else if (choice == 6) {
            double total = 0;
            for (const auto& u : users) total += u.balance;
            cout << "Total users: " << users.size() << endl;
            cout << "Total funds in system: $" << total << endl;
        }
        else if (choice == 7) {
            cout << "Parent logged out.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}


// User dashboard
void userMenu(int idx) {
    int choice;
    do {
        cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Logout\nChoice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Balance: $" << users[idx].balance << endl;
            break;
        case 2: {
            double amount;
            cout << "Enter amount to deposit: ";
            cin >> amount;
            if (amount > 0) {
                users[idx].balance += amount;
                saveUsers();
                cout << "Deposited successfully.\n";
            }
            else {
                cout << "Invalid amount.\n";
            }
            break;
        }
        case 3: {
            double amount;
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (amount > 0 && users[idx].balance >= amount) {
                users[idx].balance -= amount;
                saveUsers();
                cout << "Withdrawn successfully.\n";
            }
            else {
                cout << "Invalid amount or insufficient balance.\n";
            }
            break;
        }
        case 4: {
            string toUser;
            double amount;
            cout << "Enter recipient username: ";
            cin >> toUser;
            cout << "Enter amount to transfer: ";
            cin >> amount;

            int receiver = -1;
            for (int i = 0; i < users.size(); ++i) {
                if (users[i].username == toUser) {
                    receiver = i;
                    break;
                }
            }

            if (receiver == -1) {
                cout << "User not found.\n";
            }
            else if (amount > 0 && users[idx].balance >= amount) {
                users[idx].balance -= amount;
                users[receiver].balance += amount;
                saveUsers();
                cout << "Transferred successfully.\n";
            }
            else {
                cout << "Invalid amount or insufficient balance.\n";
            }
            break;
        }
        case 5:
            cout << "Logged out.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

// Main Menu
int main() {
    loadUsers();
    int choice;
    do {
        cout << "\n--- Cane Family Bank ---\n1. Register\n2. Login\n3. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2: {
            bool isAdmin = false;
            int idx = loginUser(isAdmin);
            if (isAdmin) adminMenu();
            else if (idx != -1) userMenu(idx);
            break;
        }
        case 3:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 3);

    return 0;
}


