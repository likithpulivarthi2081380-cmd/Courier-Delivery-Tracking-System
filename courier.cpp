#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// Base Class
class Courier {
protected:
    int id;
    char sender[50];
    char receiver[50];
    char address[100];
    char status[30];

public:
    Courier() {
        id = 0;
        strcpy(sender, "");
        strcpy(receiver, "");
        strcpy(address, "");
        strcpy(status, "Pending");
    }

    // Virtual destructor is good practice when using virtual functions
    virtual ~Courier() {}

    void input() {
        cout << "Enter Courier ID: ";
        cin >> id;
        cout << "Enter Sender Name: ";
        cin >> sender;
        cout << "Enter Receiver Name: ";
        cin >> receiver;
        cout << "Enter Delivery Address: ";
        cin.ignore();
        cin.getline(address, 100);
        strcpy(status, "Pending");
    }

    virtual void display() {
        cout << "\nID: " << id;
        cout << "\nSender: " << sender;
        cout << "\nReceiver: " << receiver;
        cout << "\nAddress: " << address;
        cout << "\nStatus: " << status << endl;
    }

    int getID() {
        return id;
    }

    void updateStatus(const char newStatus[]) {
        strncpy(status, newStatus, 29);
        status[29] = '\0'; // Ensure null termination
    }
};

// Derived Class
class ExpressCourier : public Courier {
private:
    int priority;

public:
    void input() {
        Courier::input();
        cout << "Enter Priority (1-High, 2-Medium, 3-Low): ";
        cin >> priority;
    }

    void display() override {
        Courier::display();
        cout << "Priority: " << priority << endl;
    }
};

// Save to file - Note: We save as the base Courier size to keep records uniform 
// for the update logic to work via seekp.
void saveToFile(Courier &c) {
    ofstream file("courier.txt", ios::app | ios::binary);
    if (!file) {
        cout << "Error opening file for writing!\n";
        return;
    }
    file.write(reinterpret_cast<char*>(&c), sizeof(Courier));
    file.close();
}

// Read from file
void displayFromFile() {
    Courier c;
    ifstream file("courier.txt", ios::binary);
    
    if (!file) {
        cout << "\nNo records found (file does not exist).\n";
        return;
    }

    cout << "\n--- Courier Records ---\n";
    while (file.read(reinterpret_cast<char*>(&c), sizeof(Courier))) {
        c.display();
        cout << "-----------------------";
    }
    file.close();
}

// Update status - Fixed seekp logic
void updateCourierStatus() {
    fstream file("courier.txt", ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Error: Could not open file for updating!\n";
        return;
    }

    Courier c;
    int id;
    char newStatus[30];
    bool found = false;

    cout << "Enter Courier ID to update: ";
    cin >> id;

    while (file.read(reinterpret_cast<char*>(&c), sizeof(Courier))) {
        if (c.getID() == id) {
            cout << "Current Status: " << c.getID() << " is " << (strlen(c.getID() == 0 ? "Empty" : "Active")) << endl; 
            cout << "Enter New Status (Shipped/OutForDelivery/Delivered): ";
            cin >> newStatus;

            c.updateStatus(newStatus);

            // Calculate position: move back one object size from current read position
            int pos = (int)file.tellg() - sizeof(Courier);
            file.seekp(pos, ios::beg); 
            
            file.write(reinterpret_cast<char*>(&c), sizeof(Courier));
            
            cout << "Status Updated Successfully!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Courier ID not found!\n";

    file.close();
}

// Search by ID
void trackCourier() {
    Courier c;
    ifstream file("courier.txt", ios::binary);
    int id;
    bool found = false;

    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    cout << "Enter Courier ID to track: ";
    cin >> id;

    while (file.read(reinterpret_cast<char*>(&c), sizeof(Courier))) {
        if (c.getID() == id) {
            c.display();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Courier not found!\n";

    file.close();
}

// MAIN
int main() {
    int choice;

    do {
        cout << "\n===== Courier Delivery System =====\n";
        cout << "1. Add Courier\n";
        cout << "2. Display All Couriers\n";
        cout << "3. Track Courier\n";
        cout << "4. Update Status\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int type;
            cout << "1. Normal Courier\n2. Express Courier\nChoose type: ";
            cin >> type;

            if (type == 1) {
                Courier c;
                c.input();
                saveToFile(c);
            } else {
                ExpressCourier ec;
                ec.input();
                // Note: To keep file seeking simple, we save only the base part
                saveToFile(ec); 
            }
            cout << "Courier Added Successfully!\n";
        }
        else if (choice == 2) {
            displayFromFile();
        }
        else if (choice == 3) {
            trackCourier();
        }
        else if (choice == 4) {
            updateCourierStatus();
        }

    } while (choice != 5);

    cout << "Thank You!\n";
    return 0;
}
