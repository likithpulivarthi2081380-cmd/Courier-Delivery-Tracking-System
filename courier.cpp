#include <iostream>
#include <fstream>
using namespace std;

// Base Class
class Courier {
protected:
    int id;
    string sender, receiver, address, status;

public:
    void input() {
        cout << "Enter Courier ID: ";
        cin >> id;

        cin.ignore();
        cout << "Enter Sender Name: ";
        getline(cin, sender);

        cout << "Enter Receiver Name: ";
        getline(cin, receiver);

        cout << "Enter Address: ";
        getline(cin, address);

        status = "Pending";
    }

    void display() {
        cout << "\nID: " << id;
        cout << "\nSender: " << sender;
        cout << "\nReceiver: " << receiver;
        cout << "\nAddress: " << address;
        cout << "\nStatus: " << status << endl;
    }

    int getID() {
        return id;
    }

    void updateStatus(string newStatus) {
        status = newStatus;
    }

    void saveToFile() {
        ofstream file("courier.txt", ios::app);
        file << id << "|"
             << sender << "|"
             << receiver << "|"
             << address << "|"
             << status << endl;
        file.close();
    }
};

// Display all
void displayAll() {
    ifstream file("courier.txt");
    string line;

    cout << "\n--- Courier Records ---\n";

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// Track courier
void trackCourier() {
    ifstream file("courier.txt");
    string line;
    int searchID;

    cout << "Enter ID to track: ";
    cin >> searchID;

    bool found = false;

    while (getline(file, line)) {
        int id = stoi(line.substr(0, line.find("|")));

        if (id == searchID) {
            cout << "\nFound:\n" << line << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Courier not found!\n";

    file.close();
}

// Update status
void updateStatus() {
    ifstream file("courier.txt");
    ofstream temp("temp.txt");

    string line;
    int searchID;
    string newStatus;

    cout << "Enter ID to update: ";
    cin >> searchID;

    cin.ignore();
    cout << "Enter new status: ";
    getline(cin, newStatus);

    bool found = false;

    while (getline(file, line)) {
        int id = stoi(line.substr(0, line.find("|")));

        if (id == searchID) {
            size_t pos = line.rfind("|");
            line = line.substr(0, pos + 1) + newStatus;
            found = true;
        }

        temp << line << endl;
    }

    file.close();
    temp.close();

    remove("courier.txt");
    rename("temp.txt", "courier.txt");

    if (found)
        cout << "Status updated!\n";
    else
        cout << "Courier not found!\n";
}

// MAIN
int main() {
    int choice;

    do {
        cout << "\n===== Courier System =====\n";
        cout << "1. Add Courier\n";
        cout << "2. Display All\n";
        cout << "3. Track Courier\n";
        cout << "4. Update Status\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Courier c;
            c.input();
            c.saveToFile();
            cout << "Added successfully!\n";
        }

        else if (choice == 2) {
            displayAll();
        }

        else if (choice == 3) {
            trackCourier();
        }

        else if (choice == 4) {
            updateStatus();
        }

    } while (choice != 5);

    cout << "Thank you!\n";
    return 0;
}
