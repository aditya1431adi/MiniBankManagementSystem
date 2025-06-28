#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

class Account {
private:
    int accNumber;
    char name[50];
    char accType; // S - Saving, C - Current
    float balance;

public:
    void createAccount();
    void showAccount() const;
    void modify();
    void deposit(double);
    void withdraw(double);
    void report() const;
    int getAccNumber() const;
    double getBalance() const;
    char getAccType() const;
};

// Member function definitions

void Account::createAccount() {
    cout << "\nEnter Account Number: ";
    cin >> accNumber;
    cout << "Enter Account Holder Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(name, 50);
    cout << "Enter Account Type (S/Saving, C/Current): ";
    cin >> accType;
    accType = toupper(accType);
    cout << "Enter Initial Deposit (>=500 for Saving and >=1000 for Current): ";
    cin >> balance;
    cout << "\nAccount Created Successfully!\n";
}

void Account::showAccount() const {
    cout << "\nAccount Number: " << accNumber;
    cout << "\nAccount Holder Name: " << name;
    cout << "\nType of Account: " << accType;
    cout << "\nBalance: Rs. " << balance << endl;
}

void Account::modify() {
    cout << "\nAccount Number: " << accNumber;
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Modify Account Type: ";
    cin >> accType;
    accType = toupper(accType);
    cout << "Modify Balance Amount: ";
    cin >> balance;
}

void Account::deposit(double amount) {
    balance += amount;
}

void Account::withdraw(double amount) {
    double minBalance = (accType == 'S') ? 500 : 1000;
    if (balance - amount < minBalance) {
        cout << "\nInsufficient balance! Minimum balance must be maintained: Rs. " << minBalance << endl;
    } else {
        balance -= amount;
    }
}

void Account::report() const {
    cout << accNumber << setw(10) << " " << name << setw(10) << accType << setw(10) << balance << endl;
}

int Account::getAccNumber() const { return accNumber; }
double Account::getBalance() const { return balance; }
char Account::getAccType() const { return accType; }

// Functions to handle file operations

void writeAccount() {
    Account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account ac;
    bool found = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!! Press any Key...";
        return;
    }
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        if (ac.getAccNumber() == n) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\nAccount number does not exist!";
}

void modifyAccount(int n) {
    Account ac;
    fstream File;
    bool found = false;

    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open!! Press any Key...";
        return;
    }

    while (!File.eof() && found == false) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char *>(&ac), sizeof(Account));
        if (ac.getAccNumber() == n) {
            ac.showAccount();
            cout << "\nEnter new details: " << endl;
            ac.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Account));
            cout << "\nRecord Updated.";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\nRecord Not Found!";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!! Press any Key...";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        if (ac.getAccNumber() != n) {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\nRecord Deleted.";
}

void displayAll() {
    Account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!! Press any key...";
        return;
    }

    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME        Type    Balance\n";
    cout << "====================================================\n";

    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))) {
        if (inFile.gcount() == sizeof(Account)) {
            ac.report();
        }
    }

    inFile.close();
}



void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    Account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open!! Press any Key...";
        return;
    }
    while (!File.eof() && found == false) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char *>(&ac), sizeof(Account));
        if (ac.getAccNumber() == n) {
            ac.showAccount();
            if (option == 1) {
                cout << "\nEnter amount to deposit: ";
                cin >> amt;
                ac.deposit(amt);
            }
            if (option == 2) {
                cout << "\nEnter amount to withdraw: ";
                cin >> amt;
                ac.withdraw(amt);
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Account));
            cout << "\nRecord Updated.";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\nRecord Not Found!";
}

// Main menu

int main() {
    int choice;
    int num;

    do {
        cout << "\n\n\tMAIN MENU";
        cout << "\n1. NEW ACCOUNT";
        cout << "\n2. DEPOSIT AMOUNT";
        cout << "\n3. WITHDRAW AMOUNT";
        cout << "\n4. BALANCE ENQUIRY";
        cout << "\n5. ALL ACCOUNT HOLDER LIST";
        cout << "\n6. CLOSE AN ACCOUNT";
        cout << "\n7. MODIFY AN ACCOUNT";
        cout << "\n8. EXIT";
        cout << "\nSelect Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2:
            cout << "\nEnter The account No. : "; cin >> num;
            depositWithdraw(num, 1);
            break;
        case 3:
            cout << "\nEnter The account No. : "; cin >> num;
            depositWithdraw(num, 2);
            break;
        case 4:
            cout << "\nEnter The account No. : "; cin >> num;
            displayAccount(num);
            break;
        case 5:
            displayAll();
            break;
        case 6:
            cout << "\nEnter The account No. : "; cin >> num;
            deleteAccount(num);
            break;
        case 7:
            cout << "\nEnter The account No. : "; cin >> num;
            modifyAccount(num);
            break;
        case 8:
            cout << "\nThanks for using Bank Management System!";
            break;
        default:
            cout << "\nInvalid Option!";
        }
    } while (choice != 8);

    return 0;
}
