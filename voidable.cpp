#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Contact {
    string name;
    string surname;
    string phone;
};

const int MAX_RECORDS = 20;
vector<Contact> directory;

void displayMenu();
void addRecord();
void deleteRecord();
void findRecord();
void showAllRecords();
void saveToFile();
void loadFromFile();
bool isPhoneUnique(const string& phone);

int main() {
    loadFromFile();
    int choice;

    do {
        displayMenu();
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
            case 1: addRecord(); break;
            case 2: deleteRecord(); break;
            case 3: findRecord(); break;
            case 4: showAllRecords(); break;
            case 5: saveToFile(); break;
            case 6: loadFromFile(); break;
            case 7: cout << "Вихід з програми.\n"; break;
            default: cout << "Неправильний вибір. Спробуйте ще раз.\n";
        }
    } while (choice != 7);

    return 0;
}

void displayMenu() {
    cout << "\n1. Додати запис\n"
         << "2. Видалити запис\n"
         << "3. Знайти запис\n"
         << "4. Показати всі записи\n"
         << "5. Зберегти записи у файл\n"
         << "6. Завантажити записи з файлу\n"
         << "7. Вийти із програми\n";
}

void addRecord() {
    if (directory.size() >= MAX_RECORDS) {
        cout << "Довідник заповнений. Не можна додати більше записів.\n";
        return;
    }

    Contact newContact;
    cout << "Введіть ім'я: ";
    cin >> newContact.name;
    cout << "Введіть прізвище: ";
    cin >> newContact.surname;

    do {
        cout << "Введіть номер телефону (до 12 цифр): ";
        cin >> newContact.phone;
        if (!isPhoneUnique(newContact.phone)) {
            cout << "Номер телефону повинен бути унікальним. Спробуйте ще раз.\n";
        }
    } while (!isPhoneUnique(newContact.phone));

    directory.push_back(newContact);
    cout << "Запис успішно додано.\n";
}

void deleteRecord() {
    string phone;
    cout << "Введіть номер телефону для видалення: ";
    cin >> phone;

    for (auto it = directory.begin(); it != directory.end(); ++it) {
        if (it->phone == phone) {
            directory.erase(it);
            cout << "Запис видалено.\n";
            return;
        }
    }
    cout << "Запис з таким номером не знайдено.\n";
}

void findRecord() {
    string surname;
    cout << "Введіть прізвище для пошуку: ";
    cin >> surname;

    bool found = false;
    for (const auto& contact : directory) {
        if (contact.surname == surname) {
            cout << "Ім'я: " << contact.name << ", Прізвище: " << contact.surname
                 << ", Телефон: " << contact.phone << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Запис не знайдено.\n";
    }
}

void showAllRecords() {
    if (directory.empty()) {
        cout << "Довідник порожній.\n";
        return;
    }

    cout << left << setw(15) << "Ім'я" << setw(20) << "Прізвище" << "Телефон\n";
    for (const auto& contact : directory) {
        cout << setw(15) << contact.name << setw(20) << contact.surname << contact.phone << endl;
    }
}

void saveToFile() {
    ofstream file("phone.dat", ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу для запису.\n";
        return;
    }

    size_t size = directory.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(&directory[0]), size * sizeof(Contact));
    file.close();

    cout << "Записи збережено у файл.\n";
}

void loadFromFile() {
    ifstream file("phone.dat", ios::binary);
    if (!file) {
        cout << "Файл не знайдено, створюється новий довідник.\n";
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    directory.resize(size);
    file.read(reinterpret_cast<char*>(&directory[0]), size * sizeof(Contact));
    file.close();

    cout << "Записи завантажено з файлу.\n";
}

bool isPhoneUnique(const string& phone) {
    for (const auto& contact : directory) {
        if (contact.phone == phone) {
            return false;
        }
    }
    return true;
}
