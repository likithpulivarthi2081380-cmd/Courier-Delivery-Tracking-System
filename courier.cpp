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

    void updateStatus(char newStatus[]) {
        strcpy(status, newStatus);
    }

    char* getStatus() {
        return status;
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

    void display() {
        Courier::display();
        cout << "Priority: " << priority << endl;
    }
};

// Save to file
void saveToFile(Courier c) {
    ofstream file("courier.txt", ios::app);
    file.write((char*)&c, sizeof(c));
    file.close();
}

// Read from file
void displayFromFile() {
    Courier c;
    ifstream file("courier.txt");

    cout << "\n--- Courier Records ---\n";

    while (file.read((char*)&c, sizeof(c))) {
        c.display();
    }

    file.close();
}

// Update status
void updateCourierStatus() {
    fstream file("courier.txt", ios::in | ios::out);
    Courier c;
    int id;
    char newStatus[30];
    bool found = false;

    cout << "Enter Courier ID to update: ";
    cin >> id;

    while (file.read((char*)&c, sizeof(c))) {
        if (c.getID() == id) {
            cout << "Enter New Status (Shipped/OutForDelivery/Delivered): ";
            cin >> newStatus;

            c.updateStatus(newStatus);

            int pos = (-1) * sizeof(c);
            file.seekp(pos, ios::cur);
            file.write((char*)&c, sizeof(c));

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
    ifstream file("courier.txt");
    int id;
    bool found = false;

    cout << "Enter Courier ID to track: ";
    cin >> id;

    while (file.read((char*)&c, sizeof(c))) {
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