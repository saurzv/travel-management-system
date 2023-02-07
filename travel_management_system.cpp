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

void Customer::showDetails()
{
    // cout << name << endl;
    // cout << age << endl;
    // cout << customerID << endl;
    cout << "Customer ID  : " << customerID << '\n';
    cout << "Name         : " << name << '\n';
    cout << "Gender       : " << gender << '\n';
    cout << "Age          : " << age << '\n';
    cout << "Mobile No    : " << mobileNo << '\n';
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
            c.showDetails();
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

void menu()
{
    short choice;
    cout << "----- MENU -----\n";
    cout << "1.\tNew customer\n";
    cout << "2.\tList customer\n";
    cout << "3.\tSearch customer\n";
    cout << "4.\tDelete customer\n";
    cout << "\n\n\tEnter Choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        write_customer();
        break;
    }
    case 2:
    {
        list_customer();
        break;
    }
    case 3:
    {
        cout << "Enter customer ID: ";
        int custID;
        cin >> custID;
        search_cutomer(custID);
        break;
    }
    case 4:
    {
        cout << "Enter customer ID: ";
        int custID;
        cin >> custID;
        delete_customer(custID);
        break;
    }
    }
}

int main()
{
    menu();
    // write_customer();
    return 0;
}