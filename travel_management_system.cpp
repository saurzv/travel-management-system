#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Customer
{
    char name[30];
    char gender;
    char adress[80];
    short age;
    int mobileNo;

public:
    int customerID;
    void showBasicDetails();
    void showDetails();
    void getDetails();
};

void Customer::getDetails()
{

    cout << "\n----- Enter Customer Details -----\n\n\n";

    cout << "Customer ID: ";
    cin >> customerID;
    char temp[50];
    cin.getline(temp, 50);
    cout << "Name: ";
    cin.getline(name, 30);
    cout << "Adress: ";
    cin.getline(adress, 80);
    cout << "Gender: ";
    cin >> gender;
    cout << "Age: ";
    cin >> age;
    cout << "Mobile Number: ";
    cin >> mobileNo;
}

void Customer::showBasicDetails()
{
    /* To show basic details while listing all the customoers! */
    cout << "-----------------------------------------\n";
    cout << "Customer ID\t:\t" << customerID << '\n';
    cout << "Name\t\t:\t" << name << '\n';
    cout << "-----------------------------------------\n";
}

void Customer::showDetails()
{
    // cout << name << endl;
    // cout << age << endl;
    // cout << customerID << endl;
    cout << "-----------------------------------------\n";
    cout << "Customer ID\t:\t" << customerID << '\n';
    cout << "Name\t\t:\t" << name << '\n';
    cout << "Gender\t\t:\t" << gender << '\n';
    cout << "Age\t\t:\t" << age << '\n';
    cout << "Adress\t\t:\t" << adress << '\n';
    cout << "Mobile No\t:\t" << mobileNo << '\n';
    cout << "-----------------------------------------\n";
}

void write_customer()
{
    Customer c;
    ofstream outf("customers.bin", ios::app | ios::binary);
    c.getDetails();
    // c.showDetails();
    outf.write(reinterpret_cast<char *>(&c), sizeof(Customer));
    cout << "Record inserted successfully!" << endl;
    outf.close();
}

void list_customer()
{
    Customer c;
    ifstream intf("customers.bin", ios::in | ios::binary);
    intf.seekg(0, ios::beg);
    if (!intf)
        cout << "ERR" << '\n';
    else
    {
        while (intf.read(reinterpret_cast<char *>(&c), sizeof(Customer)))
            c.showBasicDetails();
    }
}

void search_cutomer(int custID)
{
    Customer c;
    bool found = 0;
    ifstream intf("customers.bin", ios::in | ios::binary);
    if (!intf)
        cout << "ERR" << '\n';
    else
    {
        while (intf.read(reinterpret_cast<char *>(&c), sizeof(Customer)))
        {
            if (c.customerID == custID)
            {
                found = 1;
                c.showDetails();
                break;
            }
            if (!found)
                cout << "ERR" << '\n';
        }
        intf.close();
    }
}

void delete_customer(int custID)
{
    Customer c;
    bool found = 0;
    ifstream intf("customers.bin", ios::in | ios::binary);
    if (!intf)
        cout << "ERR" << '\n';
    else
    {
        ofstream outf("temp.bin", ios::app | ios::binary);
        while (intf.read(reinterpret_cast<char *>(&c), sizeof(Customer)))
        {
            if (c.customerID == custID)
                found = 1;
            else
                outf.write(reinterpret_cast<char *>(&c), sizeof(Customer));
        }
        intf.close();
        outf.close();
        if (!found)
        {
            cout << "Customer not found!" << '\n';
            remove("temp.bin");
        }
        else
        {
            remove("customers.bin");
            rename("temp.bin", "customers.bin");
            cout << "Record deleted successfully!" << '\n';
        }
    }
}

class Cab
{
protected:
    int distance;
    int passengers;
    int cab_charge;

public:
    Cab(int dist, int totalPassenger)
    {
        distance = dist;
        passengers = totalPassenger;
    }
    void suggestCabs(string name, int farePerKilo);
    int getCabCharge();
};

int Cab::getCabCharge()
{
    return cab_charge;
}

void Cab::suggestCabs(string name, int farePerKilo)
{
    bool isConfirm = 0;
    short temp;
    bool isValid;

    cab_charge = farePerKilo * distance;

    cout << "\n\tCab Provider\t: " << name << "\t\n";
    cout << "\tFare\t\t: " << cab_charge << '\n';

    do
    {
        cout << "\n\t1. Book\n\t2. Show other options\n\n\t: ";
        cin >> temp;

        isValid = (temp == 1 || temp == 2) ? 1 : 0;
        if (!isValid)
            cout << "Invalid input" << '\n';
    } while (!isValid);

    isConfirm = temp == 1 ? 1 : 0;

    switch (isConfirm)
    {
    case 0:
        cab_charge = 0;
        return;
    case 1:
        cout << "Cab booked successfully!" << '\n';
        return;
    }
}

void menu()
{
serve_menu:
    short choice;
    cout << "----- MENU -----\n";
    cout << "1.\tNew customer\n";
    cout << "2.\tList customer\n";
    cout << "3.\tSearch customer\n";
    cout << "4.\tDelete customer\n";
    cout << "5.\tBook Cab\n";
    cout << "0.\tExit the program\n";
    cout << "\n\n\tEnter Choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        write_customer();
        goto serve_menu;
    }
    case 2:
    {
        list_customer();
        goto serve_menu;
    }
    case 3:
    {
        cout << "Enter customer ID: ";
        int custID;
        cin >> custID;
        search_cutomer(custID);
        goto serve_menu;
    }
    case 4:
    {
        cout << "Enter customer ID: ";
        int custID;
        cin >> custID;
        delete_customer(custID);
        goto serve_menu;
    }
    case 5:
    {
        int dist, totalPassenger;
        do
        {
            cout << "\n\tEnter the travel distance: ";
            cin >> dist;
            cout << "\tEnter total number of passenger: ";
            cin >> totalPassenger;
            if (totalPassenger <= 0)
                cout << "Invalid number of passenger\n";
            if (dist <= 0)
                cout << "Invalid distance\n";
        } while (totalPassenger <= 0 || dist <= 0);

        Cab C(dist, totalPassenger);

        if (totalPassenger < 4)
        {
            C.suggestCabs("Small Cab Co.", 15);
            if (C.getCabCharge() == 0)
                C.suggestCabs("Large Cab Co.", 25);
        }
        else if (totalPassenger >= 4)
        {
            C.suggestCabs("Large Cab Co.", 25);
        }

        if (C.getCabCharge() == 0)
        {
            cout << "No other options!\n";
        }
        goto serve_menu;
    }
    case 0:
    {
        exit(0);
    }
    default:
    {
        cout << "Wrong Option! Please try again!\n";
        goto serve_menu;
    }
    }
}

int main()
{
    menu();
    // write_customer();
    return 0;
}