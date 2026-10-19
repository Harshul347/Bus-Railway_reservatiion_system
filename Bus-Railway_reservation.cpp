
#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int MAX_SEATS = 20;
const int SERVICE_COUNT = 4;

class Service {
public:
    int id;
    string type;
    string name;
    string source;
    string destination;
    int totalSeats;
    int fare;
};

class Booking {
public:
    int pnr;
    int serviceId;
    int seatNo;
    string passengerName;
    int age;
    Booking* next;

    Booking(int p, int sid, int seat,
            string name, int a) {
        pnr = p;
        serviceId = sid;
        seatNo = seat;
        passengerName = name;
        age = a;
        next = nullptr;
    }
};

class ReservationSystem {
private:
    Service services[SERVICE_COUNT];
    Booking* head;
    int nextPNR;

    void initializeServices() {
        services[0] = {
            101, "Bus", "Express Bus",
            "Bareilly", "Delhi", MAX_SEATS, 500
        };

        services[1] = {
            102, "Bus", "Volvo Bus",
            "Bareilly", "Lucknow", MAX_SEATS, 700
        };

        services[2] = {
            201, "Train", "Passenger Express",
            "Bareilly", "Delhi", MAX_SEATS, 350
        };

        services[3] = {
            202, "Train", "Superfast Express",
            "Bareilly", "Lucknow", MAX_SEATS, 450
        };
    }

    Service* findService(int id) {
        for (int i = 0; i < SERVICE_COUNT; i++) {
            if (services[i].id == id)
                return &services[i];
        }
        return nullptr;
    }

    Booking* findBooking(int pnr) {
        Booking* temp = head;

        while (temp != nullptr) {
            if (temp->pnr == pnr)
                return temp;

            temp = temp->next;
        }

        return nullptr;
    }

    bool isSeatBooked(int serviceId, int seatNo) {
        Booking* temp = head;

        while (temp != nullptr) {
            if (temp->serviceId == serviceId &&
                temp->seatNo == seatNo) {
                return true;
            }

            temp = temp->next;
        }

        return false;
    }

    int availableSeats(int serviceId) {
        Service* service = findService(serviceId);

        if (service == nullptr)
            return 0;

        int available = 0;

        for (int seat = 1;
             seat <= service->totalSeats; seat++) {
            if (!isSeatBooked(serviceId, seat))
                available++;
        }

        return available;
    }

public:
    ReservationSystem() {
        head = nullptr;
        nextPNR = 1001;
        initializeServices();
    }

    // 1. Display buses and trains
    void displayServices() {
        cout << "\n========== BUS & TRAIN LIST ==========\n";

        for (int i = 0; i < SERVICE_COUNT; i++) {
            cout << "\nService ID : " << services[i].id;
            cout << "\nType       : " << services[i].type;
            cout << "\nName       : " << services[i].name;
            cout << "\nFrom       : " << services[i].source;
            cout << "\nTo         : " << services[i].destination;
            cout << "\nFare       : Rs. " << services[i].fare;
            cout << "\nSeats Left : "
                 << availableSeats(services[i].id);
            cout << "\n---------------------------------------\n";
        }
    }

    // 2. Book a ticket
    void bookTicket() {
        int serviceId, seatNo, age;
        string name;

        displayServices();

        cout << "\nEnter Service ID: ";
        cin >> serviceId;

        Service* service = findService(serviceId);

        if (service == nullptr) {
            cout << "Invalid service ID!\n";
            return;
        }

        if (availableSeats(serviceId) == 0) {
            cout << "Sorry! No seats available.\n";
            return;
        }

        cout << "Enter Passenger Name: ";
        getline(cin >> ws, name);

        cout << "Enter Passenger Age: ";
        if (!(cin >> age) || age < 1 || age > 120) {
            cout << "Invalid age!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Available seat numbers: ";
        for (int s = 1; s <= service->totalSeats; s++) {
            if (!isSeatBooked(serviceId, s))
                cout << s << " ";
        }

        cout << "\nChoose Seat Number: ";
        cin >> seatNo;

        if (seatNo < 1 || seatNo > service->totalSeats) {
            cout << "Invalid seat number!\n";
            return;
        }

        if (isSeatBooked(serviceId, seatNo)) {
            cout << "Seat already booked! Choose another seat.\n";
            return;
        }

        Booking* newBooking = new Booking(
            nextPNR, serviceId, seatNo, name, age
        );

        // Insert booking at beginning of linked list
        newBooking->next = head;
        head = newBooking;

        cout << "\nTicket booked successfully!\n";
        cout << "PNR / Booking ID : " << nextPNR;
        cout << "\nPassenger        : " << name;
        cout << "\nService          : " << service->name;
        cout << "\nSeat Number      : " << seatNo;
        cout << "\nFare             : Rs. " << service->fare;
        cout << "\nPlease save your PNR.\n";

        nextPNR++;
    }

    // 3. Search ticket by PNR
    void searchTicket() {
        int pnr;

        cout << "\nEnter PNR / Booking ID: ";
        cin >> pnr;

        Booking* b = findBooking(pnr);

        if (b == nullptr) {
            cout << "Booking not found!\n";
            return;
        }

        Service* service = findService(b->serviceId);

        cout << "\n========== TICKET DETAILS ==========\n";
        cout << "PNR          : " << b->pnr << '\n';
        cout << "Passenger    : " << b->passengerName << '\n';
        cout << "Age          : " << b->age << '\n';
        cout << "Service Type : " << service->type << '\n';
        cout << "Service Name : " << service->name << '\n';
        cout << "From         : " << service->source << '\n';
        cout << "To           : " << service->destination << '\n';
        cout << "Seat Number  : " << b->seatNo << '\n';
        cout << "Fare         : Rs. " << service->fare << '\n';
    }

    // 4. Cancel ticket
    void cancelTicket() {
        int pnr;

        cout << "\nEnter PNR to cancel: ";
        cin >> pnr;

        Booking* temp = head;
        Booking* prev = nullptr;

        while (temp != nullptr && temp->pnr != pnr) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Booking not found!\n";
            return;
        }

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;

        cout << "Ticket cancelled successfully!\n";
        cout << "Seat is now available for booking.\n";
    }

    // 5. Display all bookings
    void displayBookings() {
        if (head == nullptr) {
            cout << "\nNo bookings found!\n";
            return;
        }

        Booking* temp = head;

        cout << "\n========== ALL BOOKINGS ==========\n";

        while (temp != nullptr) {
            Service* service = findService(temp->serviceId);

            cout << "\nPNR       : " << temp->pnr;
            cout << "\nPassenger : " << temp->passengerName;
            cout << "\nService   : " << service->name;
            cout << "\nSeat      : " << temp->seatNo;
            cout << "\nFare      : Rs. " << service->fare;
            cout << "\n----------------------------------\n";

            temp = temp->next;
        }
    }

    // 6. Check seat availability
    void checkSeats() {
        int serviceId;

        displayServices();

        cout << "\nEnter Service ID: ";
        cin >> serviceId;

        Service* service = findService(serviceId);

        if (service == nullptr) {
            cout << "Invalid service ID!\n";
            return;
        }

        cout << "\nAvailable seats: ";

        bool found = false;

        for (int s = 1; s <= service->totalSeats; s++) {
            if (!isSeatBooked(serviceId, s)) {
                cout << s << " ";
                found = true;
            }
        }

        if (!found)
            cout << "None";

        cout << "\nTotal available: "
             << availableSeats(serviceId) << '\n';
    }

    // 7. Total booking count
    void countBookings() {
        int count = 0;
        Booking* temp = head;

        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }

        cout << "\nTotal active bookings: " << count << '\n';
    }

    // Free allocated memory
    ~ReservationSystem() {
        while (head != nullptr) {
            Booking* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    ReservationSystem system;
    int choice;

    do {
        cout << "\n\n====================================";
        cout << "\n BUS & RAILWAY RESERVATION SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Display Buses and Trains";
        cout << "\n2. Book Ticket";
        cout << "\n3. Search Ticket by PNR";
        cout << "\n4. Cancel Ticket";
        cout << "\n5. Display All Bookings";
        cout << "\n6. Check Seat Availability";
        cout << "\n7. Count Total Bookings";
        cout << "\n0. Exit";
        cout << "\nEnter Your Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            continue;
        }

        switch (choice) {
            case 1:
                system.displayServices();
                break;

            case 2:
                system.bookTicket();
                break;

            case 3:
                system.searchTicket();
                break;

            case 4:
                system.cancelTicket();
                break;

            case 5:
                system.displayBookings();
                break;

            case 6:
                system.checkSeats();
                break;

            case 7:
                system.countBookings();
                break;

            case 0:
                cout << "Thank you for using the system!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
