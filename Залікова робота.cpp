#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

// === Component Class ===
class Component {
public:
    string name;
    string manufacturer;
    string features;
    double price;

    Component(string n, string m, string f, double p)
        : name(n), manufacturer(m), features(f), price(p) {
    }

    void print() const {
        cout << "- " << name << " (" << manufacturer << ", " << features << ") - " << price << " UAH" << endl;
    }

    string getInfo() const {
        return "- " + name + " (" + manufacturer + ", " + features + ") - " + to_string(price) + " UAH\n";
    }
};

// === Computer Class ===
class Computer {
public:
    string name;
    string configuration;
    vector<Component> components;

    Computer(string n, string conf, vector<Component> comps)
        : name(n), configuration(conf), components(comps) {
    }

    double getTotalPrice() const {
        double total = 0;
        for (const auto& comp : components)
            total += comp.price;
        return total;
    }

    void print() const {
        cout << "===============================\n";
        cout << "| COMPUTER: " << name << "\n";
        cout << "| Configuration: " << configuration << "\n";
        cout << "| Components:\n";
        for (const auto& comp : components) {
            cout << "| ";
            comp.print();
        }
        cout << "| Total Price: " << getTotalPrice() << " UAH\n";
        cout << "===============================\n\n";
    }

    string getInfo() const {
        string info = "=== " + name + " ===\n";
        info += "Configuration: " + configuration + "\n";
        for (const auto& comp : components)
            info += comp.getInfo();
        info += "Total Price: " + to_string(getTotalPrice()) + " UAH\n\n";
        return info;
    }
};

// === Employee Base Class ===
class Employee {
public:
    string lastName;
    string firstName;
    string middleName;
    string position;
    string phone;

    Employee(string ln, string fn, string mn, string pos, string ph)
        : lastName(ln), firstName(fn), middleName(mn), position(pos), phone(ph) {
    }

    virtual void printInfo() const {
        cout << "-------------------------------\n";
        cout << "| " << position << ": " << lastName << " " << firstName << " " << middleName << "\n";
        cout << "| Phone: " << phone << "\n";
        cout << "-------------------------------\n";
    }

    virtual string getInfo() const {
        return position + ": " + lastName + " " + firstName + " " + middleName + ", Phone: " + phone + "\n";
    }

    virtual ~Employee() {}
};

// === Manager Class ===
class Manager : public Employee {
public:
    vector<Computer> sales;

    Manager(string ln, string fn, string mn, string ph)
        : Employee(ln, fn, mn, "Sales Manager", ph) {
    }

    void addSale(const Computer& comp) {
        sales.push_back(comp);
    }

    double getTotalSales() const {
        double total = 0;
        for (const auto& comp : sales)
            total += comp.getTotalPrice();
        return total;
    }

    void printSalesReport() const {
        printInfo();
        for (const auto& comp : sales)
            comp.print();
        cout << "Total Sales: " << getTotalSales() << " UAH\n\n";
    }

    string getInfo() const override {
        string info = Employee::getInfo();
        info += "Sales:\n";
        for (const auto& comp : sales)
            info += comp.getInfo();
        info += "Total Sales: " + to_string(getTotalSales()) + " UAH\n\n";
        return info;
    }
};

// === Store Class ===
class Store {
public:
    vector<Component> components;
    vector<Computer> computers;
    vector<Employee*> staff;

    ~Store() {
        for (auto emp : staff)
            delete emp;
    }

    void appendToFile(const string& filename, const string& data) const {
        ofstream file(filename, ios::app);
        if (file)
            file << data << endl;
    }

    void addComponent(const Component& comp) {
        components.push_back(comp);
    }

    void addComputer(const Computer& comp) {
        computers.push_back(comp);
    }

    void hireEmployee(Employee* emp) {
        staff.push_back(emp);
        appendToFile("PCshop.txt", "Hired: " + emp->getInfo());
    }

    void fireEmployee(const string& lastName) {
        for (auto it = staff.begin(); it != staff.end(); ++it) {
            if ((*it)->lastName == lastName) {
                appendToFile("PCshop.txt", "Fired: " + (*it)->getInfo());
                delete* it;
                staff.erase(it);
                cout << "Employee fired: " << lastName << endl;
                return;
            }
        }
        cout << "Employee with last name '" << lastName << "' not found.\n";
    }

    void sortStaffByLastName() {
        sort(staff.begin(), staff.end(), [](Employee* a, Employee* b) {
            return a->lastName < b->lastName;
            });
        cout << "Staff sorted by last name.\n";
    }

    void sortComputersByPrice() {
        sort(computers.begin(), computers.end(), [](const Computer& a, const Computer& b) {
            return a.getTotalPrice() < b.getTotalPrice();
            });
        cout << "Computers sorted by price.\n";
    }

    void printAllStaff() const {
        cout << "\n========= STORE STAFF =========\n";
        for (const auto& emp : staff)
            emp->printInfo();
        cout << endl;
    }

    void printAllComputers() const {
        cout << "\n========= AVAILABLE COMPUTERS =========\n";
        for (size_t i = 0; i < computers.size(); ++i) {
            cout << "Computer #" << i + 1 << ":\n";
            computers[i].print();
        }
    }

    void customerPurchase() {
        printAllComputers();
        cout << "Enter the number of the computer to purchase: ";
        int num;
        cin >> num;
        if (num < 1 || num > computers.size()) {
            cout << "Invalid number!\n";
            return;
        }

        Computer selectedPC = computers[num - 1];
        string customerName;
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customerName);

        string receipt = "\n=== RECEIPT ===\nCustomer: " + customerName + "\n" + selectedPC.getInfo();
        cout << receipt;

        appendToFile("Receipts.txt", receipt);
    }

    void printSalesReports() const {
        cout << "\n========= MANAGERS' SALES REPORTS =========\n";
        for (const auto& emp : staff) {
            if (Manager* mgr = dynamic_cast<Manager*>(emp)) {
                mgr->printSalesReport();
            }
        }
    }

    void saveToFile() const {
        ofstream file("PCshop.txt", ios::out);
        if (!file) {
            cout << "Error saving to file.\n";
            return;
        }

        file << "=== STAFF LIST ===\n";
        for (const auto& emp : staff)
            file << emp->getInfo() << endl;

        file << "\n=== COMPUTERS LIST ===\n";
        for (const auto& comp : computers)
            file << comp.getInfo() << endl;

        file.close();
        cout << "All store data saved to PCshop.txt\n";
    }
};

// === Main Program ===
int main() {
    Store store;

    // Predefined Components
    Component cpu("AMD Ryzen 9", "AMD", "16-core", 18000);
    Component ram("DDR5 64GB", "Corsair", "6000MHz", 9500);
    Component gpu("RTX 4090", "NVIDIA", "24GB GDDR6X", 55000);
    Component ssd("2TB SSD", "Samsung", "NVMe Gen4", 8000);
    Component psu("850W PSU", "Seasonic", "Gold Certified", 4000);
    Component case1("Full Tower RGB", "CoolerMaster", "Tempered Glass", 3000);

    Component cpu2("Intel i9", "Intel", "24-core", 20000);
    Component ram2("DDR5 32GB", "Kingston", "5600MHz", 7000);
    Component gpu2("RTX 4080", "NVIDIA", "16GB GDDR6X", 45000);
    Component ssd2("1TB SSD", "WD", "NVMe Gen3", 5000);
    Component psu2("750W PSU", "Corsair", "Bronze Certified", 3000);
    Component case2("Mid Tower", "NZXT", "White Edition", 2500);

    store.addComponent(cpu);
    store.addComponent(ram);
    store.addComponent(gpu);
    store.addComponent(ssd);
    store.addComponent(psu);
    store.addComponent(case1);
    store.addComponent(cpu2);
    store.addComponent(ram2);
    store.addComponent(gpu2);
    store.addComponent(ssd2);
    store.addComponent(psu2);
    store.addComponent(case2);

    // Predefined Computers
    Computer gamingPC("Gaming Beast PC", "Ryzen 9, 64GB RAM, RTX 4090, 2TB SSD", { cpu, ram, gpu, ssd, psu, case1 });
    Computer workPC("Workstation Pro", "Intel i9, 32GB RAM, RTX 4080, 1TB SSD", { cpu2, ram2, gpu2, ssd2, psu2, case2 });

    store.addComputer(gamingPC);
    store.addComputer(workPC);

    // Predefined Staff
    Manager* manager1 = new Manager("Shevchenko", "Olena", "Ivanivna", "0501234567");
    manager1->addSale(gamingPC);
    store.hireEmployee(manager1);
    store.hireEmployee(new Employee("Kovalenko", "Andrii", "Petrovych", "Cashier", "0502345678"));
    store.hireEmployee(new Employee("Melnyk", "Vasyl", "Serhiyovych", "PC Assembler", "0503456789"));
    store.hireEmployee(new Employee("Tkachenko", "Iryna", "Volodymyrivna", "Consultant", "0504567890"));

    int choice;
    do {
        cout << "\n======= PC SHOP MENU =======\n";
        cout << "1 - Show all computers\n";
        cout << "2 - Show all staff\n";
        cout << "3 - Buy a computer\n";
        cout << "4 - Hire a new employee\n";
        cout << "5 - Fire an employee\n";
        cout << "6 - Sort computers by price\n";
        cout << "7 - Sort staff by last name\n";
        cout << "8 - Show managers' sales reports\n";
        cout << "9 - Save all current store data to file\n";
        cout << "0 - Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            store.printAllComputers();
            break;
        case 2:
            store.printAllStaff();
            break;
        case 3:
            store.customerPurchase();
            break;
        case 4: {
            string ln, fn, mn, pos, ph;
            cout << "Enter last name: "; cin >> ln;
            cout << "Enter first name: "; cin >> fn;
            cout << "Enter middle name: "; cin >> mn;
            cout << "Enter position: "; cin.ignore(); getline(cin, pos);
            cout << "Enter phone number: "; getline(cin, ph);
            store.hireEmployee(new Employee(ln, fn, mn, pos, ph));
            break;
        }
        case 5: {
            string ln;
            cout << "Enter last name of employee to fire: ";
            cin >> ln;
            store.fireEmployee(ln);
            break;
        }
        case 6:
            store.sortComputersByPrice();
            break;
        case 7:
            store.sortStaffByLastName();
            break;
        case 8:
            store.printSalesReports();
            break;
        case 9:
            store.saveToFile();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
