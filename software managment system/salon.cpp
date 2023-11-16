#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <unordered_map>

using namespace std;

class User {
public:
    string username;
    string password;
    int loyaltyPoints;

    User() : username(""), password(""), loyaltyPoints(0) {}

    User(string name, string pass)
        : username(name), password(pass), loyaltyPoints(0) {}
};

class Appointment {
public:
    string customerName;
    string service;
    string appointmentTime;
    bool isPaid;
    string feedback;

    Appointment(string name, string serv, string time)
        : customerName(name), service(serv), appointmentTime(time), isPaid(false), feedback("") {}
};

class Salon {
private:
    vector<Appointment> appointments;
    unordered_map<string, User> users;
    User currentUser;

public:
    User getCurrentUser() const {
        return currentUser;
    }

    Salon() : currentUser() {
        loadUsersFromFile();
    }

    ~Salon() {
        saveUsersToFile();
    }

    bool registerUser(const User& user);
    bool loginUser(const string& username, const string& password);
    void logoutUser();
    void addAppointment(const Appointment& appointment);
    void cancelAppointment();
    void displayAppointments();
    void makePayment(int index);
    void provideFeedback(int index);
    void redeemPoints(); // Added function

private:
    void loadUsersFromFile();
    void saveUsersToFile();
};

bool Salon::registerUser(const User& user) {
    pair<unordered_map<string, User>::iterator, bool> result = users.insert(make_pair(user.username, user));
    if (result.second) {
        cout << "User registered successfully." << endl;
        saveUsersToFile();
        return true;
    } else {
        cout << "Username already exists. Please choose a different one." << endl;
        return false;
    }
}

bool Salon::loginUser(const string& username, const string& password) {
    unordered_map<string, User>::iterator it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        cout << "Login successful. Welcome, " << username << "!" << endl;
        currentUser = it->second;
        return true;
    } else {
        cout << "Login failed. Please check your username and password." << endl;
        return false;
    }
}

void Salon::logoutUser() {
    currentUser = User();
    cout << "Logout successful. Goodbye!" << endl;
}

void Salon::addAppointment(const Appointment& appointment) {
    if (!currentUser.username.empty()) {
        appointments.push_back(appointment);
        cout << "Appointment added successfully." << endl;
    } else {
        cout << "Error: Please log in before adding an appointment." << endl;
    }
}

void Salon::cancelAppointment() {
    if (appointments.empty()) {
        cout << "No appointments available to cancel." << endl;
    } else {
        cout << "Enter the index of the appointment to cancel: ";
        int index;
        cin >> index;

        if (index >= 0 && index < appointments.size()) {
            appointments.erase(appointments.begin() + index);
            cout << "Appointment canceled successfully." << endl;
        } else {
            cout << "Invalid index. No appointment canceled." << endl;
        }
    }
}

void Salon::displayAppointments() {
    if (appointments.empty()) {
        cout << "No appointments available." << endl;
    } else {
        cout << setw(5) << "Index" << setw(20) << "Customer Name" << setw(20) << "Service" << setw(20) << "Appointment Time" << setw(10) << "Paid" << setw(30) << "Feedback" << setw(15) << "Loyalty Points" << endl;
        for (size_t i = 0; i < appointments.size(); ++i) {
            cout << setw(5) << i << setw(20) << appointments[i].customerName << setw(20) << appointments[i].service
                 << setw(20) << appointments[i].appointmentTime << setw(10) << (appointments[i].isPaid ? "Yes" : "No")
                 << setw(30) << appointments[i].feedback << setw(15) << currentUser.loyaltyPoints << endl;
        }
    }
}

void Salon::makePayment(int index) {
    if (index >= 0 && index < appointments.size()) {
        appointments[index].isPaid = true;
        currentUser.loyaltyPoints += 20; // Award 20 points for each payment

        cout << "Payment successful. You have earned 20 loyalty points." << endl;

        if (currentUser.loyaltyPoints >= 100) {
            int redeemedAmount = currentUser.loyaltyPoints / 100; // 1 point = 1 rupee
            currentUser.loyaltyPoints %= 100;
            cout << "Congratulations! Your loyalty points have been redeemed for Rs." << redeemedAmount << " online balance." << endl;
        }
    } else {
        cout << "Invalid index. Payment failed." << endl;
    }
}

void Salon::provideFeedback(int index) {
    if (index >= 0 && index < appointments.size()) {
        cout << "Enter your feedback: ";
        string feedback;
        cin.ignore();
        getline(cin, feedback);
        appointments[index].feedback = feedback;
        cout << "Thank you for your feedback." << endl;
    } else {
        cout << "Invalid index. Feedback not provided." << endl;
    }
}

void Salon::redeemPoints() {
    if (currentUser.loyaltyPoints >= 100) {
        int redeemedAmount = currentUser.loyaltyPoints / 100; // 1 point = 1 rupee
        currentUser.loyaltyPoints %= 100;
        cout << "Congratulations! Your loyalty points have been redeemed for Rs." << redeemedAmount << " online balance." << endl;
    } else {
        cout << "Insufficient loyalty points to redeem." << endl;
    }
}

void Salon::loadUsersFromFile() {
    ifstream file("users.txt");
    if (file.is_open()) {
        string username, password;
        int loyaltyPoints;
        while (file >> username >> password >> loyaltyPoints) {
            User user(username, password);
            user.loyaltyPoints = loyaltyPoints;
            users.insert(make_pair(username, user));
        }
        file.close();
    }
}

void Salon::saveUsersToFile() {
    ofstream file("users.txt");
    if (file.is_open()) {
        for (const auto& pair : users) {
            file << pair.second.username << " " << pair.second.password << " " << pair.second.loyaltyPoints << endl;
        }
        file.close();
    }
}

int main() {
    Salon salon;
    int choice;

    while (true) {
        cout << "\nSalon Management System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Add Appointment\n";
        cout << "4. Cancel Appointment\n";
        cout << "5. Display Appointments\n";
        cout << "6. Make Payment\n";
        cout << "7. Provide Feedback\n";
        cout << "8. Redeem Loyalty Points\n";
        cout << "9. Logout\n";
        cout << "10. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                User user(username, password);
                salon.registerUser(user);
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                salon.loginUser(username, password);
                break;
            }
            case 3: {
                if (salon.getCurrentUser().username.empty()) {
                    cout << "Error: Please log in before adding an appointment." << endl;
                } else {
                    string customerName, service, appointmentTime;
                    cout << "Enter customer name: ";
                    cin.ignore();
                    getline(cin, customerName);
                    cout << "Enter service: ";
                    getline(cin, service);
                    cout << "Enter appointment time: ";
                    getline(cin, appointmentTime);
                    Appointment appointment(customerName, service, appointmentTime);
                    salon.addAppointment(appointment);
                }
                break;
            }
            case 4:
                salon.cancelAppointment();
                break;
            case 5:
                salon.displayAppointments();
                break;
            case 6: {
                if (salon.getCurrentUser().username.empty()) {
                    cout << "Error: Please log in before making a payment." << endl;
                } else {
                    int index;
                    cout << "Enter the index of the appointment to make a payment: ";
                    cin >> index;
                    salon.makePayment(index);
                }
                break;
            }
            case 7: {
                if (salon.getCurrentUser().username.empty()) {
                    cout << "Error: Please log in before providing feedback." << endl;
                } else {
                    int index;
                    cout << "Enter the index of the appointment to provide feedback: ";
                    cin >> index;
                    salon.provideFeedback(index);
                }
                break;
            }
            case 8:
                salon.redeemPoints();
                break;
            case 9:
                salon.logoutUser();
                break;
            case 10:
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return  0;
}