#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

class Booking {
protected:
    int bookingID;
    string customerName;
    string roomType;
    string bookingDate;
    string checkInDate;
    string checkOutDate;
    double roomRate;
    string status;
    string associatedCustomer; 

public:
    Booking(int id, const string& name, const string& type, const string& bookingDate, const string& checkInDate,
            const string& checkOutDate, double rate, const string& status = "Confirmed", const string& customer = "")
        : bookingID(id), customerName(name), roomType(type), bookingDate(bookingDate),
          checkInDate(checkInDate), checkOutDate(checkOutDate), roomRate(rate), status(status), associatedCustomer(customer) {}

    int getBookingID() const { return bookingID; }
    string getCustomerName() const { return customerName; }
    string getRoomType() const { return roomType; }
    string getBookingDate() const { return bookingDate; }
    string getCheckInDate() const { return checkInDate; }
    string getCheckOutDate() const { return checkOutDate; }
    double getRoomRate() const { return roomRate; }
    string getStatus() const { return status; }
    string getAssociatedCustomer() const { return associatedCustomer; }
    void setStatus(const string& newStatus) { status = newStatus; }

    virtual int getStayDuration() const {
        int checkInYear, checkInMonth, checkInDay;
        int checkOutYear, checkOutMonth, checkOutDay;
        stringstream checkInStream(checkInDate);
        stringstream checkOutStream(checkOutDate);
        char dash;
        checkInStream >> checkInYear >> dash >> checkInMonth >> dash >> checkInDay;
        checkOutStream >> checkOutYear >> dash >> checkOutMonth >> dash >> checkOutDay;
        int checkInTotalDays = checkInYear * 365 + checkInMonth * 30 + checkInDay;
        int checkOutTotalDays = checkOutYear * 365 + checkOutMonth * 30 + checkOutDay;
        return checkOutTotalDays - checkInTotalDays;
    }

    virtual void printBookingInfo() const {
        cout << "Booking ID: " << bookingID << "\n"
             << "Customer Name: " << customerName << "\n"
             << "Room Type: " << roomType << "\n"
             << "Booking Date: " << bookingDate << "\n"
             << "Check-in Date: " << checkInDate << "\n"
             << "Check-out Date: " << checkOutDate << "\n"
             << "Room Rate: $" << roomRate << "\n"
             << "Status: " << status << "\n\n";
    }
};

class HotelReservationSystem {
private:
    vector<Booking*> bookings;
    int maxCapacity;

public:
    HotelReservationSystem(int capacity) : maxCapacity(capacity) {}

    ~HotelReservationSystem() {
        for (auto booking : bookings) {
            delete booking;
        }
    }

    bool insertBooking(Booking* newBooking) {
        if (bookings.size() < maxCapacity) {
            bookings.push_back(newBooking);
            cout << "Booking added successfully.\n";
            return true;
        } else {
            cout << "Hotel is fully booked.\n";
            return false;
        }
    }
    bool deleteBooking(int bookingID, const string& customerName) 
    {
    auto it = find_if(bookings.begin(), bookings.end(), [bookingID, customerName](Booking* booking) {
        return booking->getBookingID() == bookingID && booking->getAssociatedCustomer() == customerName;
    });
    if (it != bookings.end()) 
    {
        delete *it;  
        bookings.erase(it); 
        cout << "Booking deleted successfully.\n";
        return true;
    } 
    else 
    {
        cout << "Booking not found or you're not authorized to delete this booking.\n";
        return false;
    }
    }
    bool updateBookingStatus(int bookingID, const string& newStatus, const string& customerName) {
        for (auto& booking : bookings) {
            if (booking->getBookingID() == bookingID && booking->getAssociatedCustomer() == customerName) {
                booking->setStatus(newStatus);
                cout << "Booking status updated to " << newStatus << ".\n";
                return true;
            }
        }
        cout << "Booking not found or you're not authorized to update this booking.\n";
        return false;
    }

    void sortBookingsByStayDuration() 
    {
        sort(bookings.begin(), bookings.end(), [](Booking* a, Booking* b) {
            return a->getStayDuration() > b->getStayDuration();
        });
        cout << "Bookings sorted by stay duration.\n";
    }

   Booking* getLongestStayBooking() const 
    {
    if (bookings.empty()) 
    {
        cout << "No bookings available.\n";
        return nullptr;
    }
    auto longestStay = max_element(bookings.begin(), bookings.end(),
        [](Booking* a, Booking* b) {
            return a->getStayDuration() < b->getStayDuration();
        });
    return *longestStay;
    }


    void printBookings(const string& customerName) const {
        bool found = false;
        for (const auto& booking : bookings) {
            if (booking->getAssociatedCustomer() == customerName) {
                booking->printBookingInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No bookings found for " << customerName << ".\n";
        }
    }

    bool isFull() const {
        return bookings.size() >= maxCapacity;
    }

    int getNumBookings() const {
        return bookings.size();
    }
};


Booking* createBooking() {
    int id;
    string name, type, bookingDate, checkInDate, checkOutDate;
    double rate;
    string status;
    cout << "Enter Booking ID: ";
    cin >> id;
    cout << "Enter Customer Name: ";
    cin.ignore(); 
    getline(cin, name);
    cout << "Enter Room Type (Single/Double/Suite): ";
    getline(cin, type);
    cout << "Enter Booking Date (YYYY-MM-DD): ";
    getline(cin, bookingDate);
    cout << "Enter Check-in Date (YYYY-MM-DD): ";
    getline(cin, checkInDate);
    cout << "Enter Check-out Date (YYYY-MM-DD): ";
    getline(cin, checkOutDate);
    cout << "Enter Room Rate: ";
    cin >> rate;
    cout << "Enter Booking Status (Confirmed/Pending/Cancelled): ";
    cin.ignore();  
    getline(cin, status);
    cout << "Enter Associated Customer Name: ";
    string customer;
    getline(cin, customer);
    return new Booking(id, name, type, bookingDate, checkInDate, checkOutDate, rate, status, customer);
}

int main() {
    int maxCapacity;
    cout << "Enter the maximum booking capacity for the hotel: ";
    cin >> maxCapacity;
    cin.ignore();

    HotelReservationSystem hotel(maxCapacity);

   
    int choice;
    do {
        cout << "\n--- Hotel Reservation System ---\n";
        cout << "1. Add a booking\n";
        cout << "2. View bookings for a customer\n";
        cout << "3. Update booking status\n";
        cout << "4. Delete a booking\n";
        cout << "5. View booking with the longest stay\n";
        cout << "6. Show total bookings and check if hotel is full\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();  

        switch (choice) {
            case 1: {
                Booking* newBooking = createBooking();
                hotel.insertBooking(newBooking);
                break;
            }
            case 2: {
                string customerName;
                cout << "Enter customer name to view bookings: ";
                getline(cin, customerName);
                hotel.printBookings(customerName);
                break;
            }
            case 3: {
                int bookingID;
                string newStatus, customerName;
                cout << "Enter Booking ID to update status: ";
                cin >> bookingID;
                cin.ignore();
                cout << "Enter your name to confirm identity: ";
                getline(cin, customerName);
                cout << "Enter new status (Confirmed, Pending, Cancelled): ";
                getline(cin, newStatus);
                hotel.updateBookingStatus(bookingID, newStatus, customerName);
                break;
            }
            case 4: {
                int bookingID;
                string customerName;
                cout << "Enter Booking ID to delete: ";
                cin >> bookingID;
                cin.ignore();
                cout << "Enter your name to confirm identity: ";
                getline(cin, customerName);
                hotel.deleteBooking(bookingID, customerName);
                break;
            }
            case 5: {
                Booking* longestStayBooking = hotel.getLongestStayBooking();
                if (longestStayBooking) {
                    cout << "\n--- Booking with the Longest Stay ---\n";
                    longestStayBooking->printBookingInfo();
                }
                break;
            }
            case 6: {
                cout << "\nTotal Bookings: " << hotel.getNumBookings() << "\n";
                cout << "Is the hotel full? " << (hotel.isFull() ? "Yes" : "No") << "\n";
                break;
            }
            case 0: {
                cout << "Exiting the system.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}