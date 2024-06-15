#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <unordered_map> // Подібна до map, але зберігає дані у неупорядкованому вигляді, 
                        //що може забезпечувати швидкий доступ до даних.
#include <cstring> // Містить функції для роботи з рядками символів
#include "User.h"
#include "Admin.h"
#include "Test.h"
using namespace std;
const char n = '\n';

// Клас для системи
class System {
private:
    vector<User> users; // вектор користувачів
    vector<Test> tests; // вектор тестів
    Admin admin; // дані адміністратора
    bool isAdminSet = false; // прапорець для визначення, чи встановлений адмін
    unordered_map<string, vector<string>> user_results; // результати користувачів

    // Перевірка валідності пароля (наявність хоча б однієї великої літери та цифри)
    bool validPassword(const string& password) {
        bool hasUpper = false;
        bool hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (isdigit(c)) hasDigit = true;
            if (hasUpper && hasDigit) return true;
        }
        return false;
    }

    // Шифрування методом Цезаря
    string encryptCaesar(string text, int shift) {
        string result = "";
        for (char& c : text) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                c = (c - base + shift) % 26 + base;
            }
            result += c;
        }
        return result;
    }

    // Пошук користувача за логіном
    User* searchUser(const string& login) {
        for (User& user : users) {
            if (user.login == login) {
                return &user;
            }
        }
        return nullptr;
    }

    // Пошук тесту за назвою
    Test* searchTest(const string& testName) {
        for (Test& test : tests) {
            if (test.name == testName) {
                return &test;
            }
        }
        return nullptr;
    }

    // Реєстрація нового користувача
    void registerUser() {
        User newUser;
        do {
            cout << "Name of user? ";
            cin >> newUser.name;
            // Перевірка, чи містить ім'я цифри
            if (any_of(newUser.name.begin(), newUser.name.end(), ::isdigit)) {
                cout << "The test taker's name cannot contain numbers! Please, try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the name of your village/town/city ";
            cin >> newUser.location;
            // Перевірка, чи містить назва локації цифри
            if (any_of(newUser.location.begin(), newUser.location.end(), ::isdigit)) {
                cout << "The location name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the name of your street ";
            cin >> newUser.street;
            // Перевірка, чи містить назва вулиці цифри
            if (any_of(newUser.street.begin(), newUser.street.end(), ::isdigit)) {
                cout << "The street name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the number of your house ";
            cin >> newUser.house_number;
            // Перевірка, чи містить номер будинку тільки цифри
            if (none_of(newUser.house_number.begin(), newUser.house_number.end(), ::isdigit)) {
                cout << "The house number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the number of your phone ";
            cin >> newUser.num;
            // Перевірка, чи містить номер телефону тільки цифри
            if (none_of(newUser.num.begin(), newUser.num.end(), ::isdigit)) {
                cout << "The phone number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "ENTER LOGIN ";
            cin >> newUser.login;
            // Перевірка, чи не зайнятий логін
            if (searchUser(newUser.login)) {
                cout << "This login is already taken. Please, try another one.\n";
                continue;
            }
            cout << "ENTER PASSWORD ";
            cin >> newUser.password;
            // Перевірка валідності пароля
            if (!validPassword(newUser.password)) {
                cout << "Password must contain at least one digit and one uppercase letter. Try again!" << endl;
                continue;
            }
            break;
        } while (true);

        // Шифрування логіну та пароля методом Цезаря
        int shift = 3;
        newUser.login = encryptCaesar(newUser.login, shift);
        newUser.password = encryptCaesar(newUser.password, shift);

        // Додавання нового користувача до списку користувачів
        users.push_back(newUser);
        cout << "Registration completed successfully!" << n;
        saveUsersToFile();  // Зберігаємо користувача у файл
    }

    // Збереження списку користувачів у файл
    void saveUsersToFile() {
        ofstream userFile("users.txt");
        if (userFile.is_open()) {
            for (const auto& user : users) {
                userFile << "Name: " << user.name << ", Location: " << user.location << ", Street: " << user.street << ", House Number: " << user.house_number << ", Phone Number: " << user.num << ", Login: " << user.login << ", Password: " << user.password << "\n";
            }
            userFile.close();
        }
        else {
            cout << "Unable to open users file.";
        }
    }

    // Збереження облікових даних адміністратора у файл
    void saveAdminToFile() {
        ofstream adminFile("admin.txt");
        if (adminFile.is_open()) {
            adminFile << "Name: " << admin.name << ", Location: " << admin.location << ", Street: " << admin.street << ", House Number: " << admin.house_number << ", Phone Number: " << admin.num << ", Login: " << admin.login << ", Password: " << admin.password << "\n";
            adminFile.close();
        }
        else {
            cout << "Unable to open admin file.";
        }
    }

    // Завантаження облікових даних адміністратора з файлу
    void loadAdminFromFile() {
        ifstream adminFile("admin.txt");
        if (adminFile.is_open()) {
            string line;
            getline(adminFile, line);

            size_t pos;
            pos = line.find("Name: ");
            if (pos != string::npos) { //статична константа яка представляє максимально можливе значення для типу size_t 
                pos += 6;
                size_t endPos = line.find(", Location: ", pos);
                admin.name = line.substr(pos, endPos - pos);
            }

            pos = line.find("Location: ");
            if (pos != string::npos) { 
                pos += 10;
                size_t endPos = line.find(", Street: ", pos);
                admin.location = line.substr(pos, endPos - pos);
            }

            pos = line.find("Street: ");
            if (pos != string::npos) {
                pos += 8;
                size_t endPos = line.find(", House Number: ", pos);
                admin.street = line.substr(pos, endPos - pos);
            }

            pos = line.find("House Number: ");
            if (pos != string::npos) {
                pos += 14;
                size_t endPos = line.find(", Phone Number: ", pos);
                admin.house_number = line.substr(pos, endPos - pos);
            }

            pos = line.find("Phone Number: ");
            if (pos != string::npos) {
                pos += 14;
                size_t endPos = line.find(", Login: ", pos);
                admin.num = line.substr(pos, endPos - pos);
            }

            pos = line.find("Login: ");
            if (pos != string::npos) {
                pos += 7;
                size_t endPos = line.find(", Password: ", pos);
                admin.login = line.substr(pos, endPos - pos);
            }

            pos = line.find("Password: ");
            if (pos != string::npos) {
                pos += 10;
                admin.password = line.substr(pos);
            }

            isAdminSet = true;
            adminFile.close();
        }
        else {
            cout << "Unable to open admin file.";
        }
    }

    // Завантаження списку користувачів з файлу
    void loadUsersFromFile() {
        ifstream userFile("users.txt");
        if (userFile.is_open()) {
            string line;
            while (getline(userFile, line)) {
                // Парсимо рядок і створюємо нового користувача
                string name, location, street, house_number, num, login, password;
                size_t pos;

                pos = line.find("Name: ");
                if (pos != string::npos) {
                    pos += 6;
                    size_t endPos = line.find(", Location: ", pos);
                    name = line.substr(pos, endPos - pos);
                }

                pos = line.find("Location: ");
                if (pos != string::npos) {
                    pos += 10;
                    size_t endPos = line.find(", Street: ", pos);
                    location = line.substr(pos, endPos - pos);
                }

                pos = line.find("Street: ");
                if (pos != string::npos) {
                    pos += 8;
                    size_t endPos = line.find(", House Number: ", pos);
                    street = line.substr(pos, endPos - pos);
                }

                pos = line.find("House Number: ");
                if (pos != string::npos) {
                    pos += 14;
                    size_t endPos = line.find(", Phone Number: ", pos);
                    house_number = line.substr(pos, endPos - pos);
                }

                pos = line.find("Phone Number: ");
                if (pos != string::npos) {
                    pos += 14;
                    size_t endPos = line.find(", Login: ", pos);
                    num = line.substr(pos, endPos - pos);
                }

                pos = line.find("Login: ");
                if (pos != string::npos) {
                    pos += 7;
                    size_t endPos = line.find(", Password: ", pos);
                    login = line.substr(pos, endPos - pos);
                }

                pos = line.find("Password: ");
                if (pos != string::npos) {
                    pos += 10;
                    password = line.substr(pos);
                }

                // Створюємо об'єкт користувача і додаємо його до вектора користувачів
                User loadedUser(name, location, street, house_number, num, login, password);
                users.push_back(loadedUser);
            }
            userFile.close();
        }
        else {
            cout << "Unable to open file.";
        }
    }

    // Завантаження списку тестів з файлу
    void loadTestsFromFile() {
        tests.clear(); // Очистка попередніх тестів перед завантаженням нових

        ifstream inFile("saved_tests.txt");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                string category = line;
                getline(inFile, line);
                string testName = line;

                Test test(category, testName);

                size_t numQuestions;
                inFile >> numQuestions;
                inFile.ignore(); // Ігноруємо залишок рядка після числа

                for (size_t i = 0; i < numQuestions; ++i) {
                    getline(inFile, line);
                    string questionText = line;

                    test.questions.push_back(questionText);

                    size_t numChoices;
                    inFile >> numChoices;
                    inFile.ignore(); // Ігноруємо залишок рядка після числа

                    vector<string> choices;
                    for (size_t j = 0; j < numChoices; ++j) {
                        getline(inFile, line);
                        choices.push_back(line);
                    }

                    test.answer_choices.push_back(choices);

                    size_t correctAnswerIndex;
                    inFile >> correctAnswerIndex;
                    inFile.ignore(); // Ігноруємо залишок рядка після числа

                    test.correct_answers_index.push_back(correctAnswerIndex);
                }

                tests.push_back(test);
            }
            inFile.close();
        }
        else {
            cout << "Unable to open tests file." << endl;
        }
    }

    // Збереження списку тестів у файл
    void saveTestsToFile() {
        ofstream outFile("saved_tests.txt");
        if (outFile.is_open()) {
            for (const auto& test : tests) {
                outFile << test.category << endl;
                outFile << test.name << endl;
                outFile << test.questions.size() << endl;

                for (size_t i = 0; i < test.questions.size(); ++i) {
                    outFile << test.questions[i] << endl;
                    outFile << test.answer_choices[i].size() << endl;

                    for (const auto& choice : test.answer_choices[i]) {
                        outFile << choice << endl;
                    }

                    outFile << test.correct_answers_index[i] << endl;
                }
            }
            outFile.close();
        }
        else {
            cout << "Unable to open output file." << endl;
        }
    }

public:
    // Конструктор системи, завантажує дані при створенні
    System() {
        loadUsersFromFile(); // Завантажуємо користувачів з файлу при створенні системи
        loadAdminFromFile();// Завантажуємо адміністратора з файлу
        loadTestsFromFile(); //Завантажуємо тести  
    }

    User* currentUser = nullptr; // Поточний користувач, що увійшов до системи

    // Реєстрація адміністратора
    void registerAdmin() {
        if (isAdminSet) {
            cout << "Admin already registered!" << n;
            return;
        }

        Admin newAdmin;
        // Введення даних адміністратора з перевіркою на валідність
        do {
            cout << "Enter the admin's name: ";
            cin >> newAdmin.name;
            if (any_of(newAdmin.name.begin(), newAdmin.name.end(), ::isdigit)) {
                cout << "The admin's name cannot contain numbers! Please, try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's location: ";
            cin >> newAdmin.location;
            if (any_of(newAdmin.location.begin(), newAdmin.location.end(), ::isdigit)) {
                cout << "The location name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's street: ";
            cin >> newAdmin.street;
            if (any_of(newAdmin.street.begin(), newAdmin.street.end(), ::isdigit)) {
                cout << "The street name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's house number: ";
            cin >> newAdmin.house_number;
            if (none_of(newAdmin.house_number.begin(), newAdmin.house_number.end(), ::isdigit)) {
                cout << "The house number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's phone number: ";
            cin >> newAdmin.num;
            if (none_of(newAdmin.num.begin(), newAdmin.num.end(), ::isdigit)) {
                cout << "The phone number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's login: ";
            cin >> newAdmin.login;
            if (!validPassword(newAdmin.login)) {
                cout << "Login must contain at least one digit and one uppercase letter. Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the admin's password: ";
            cin >> newAdmin.password;
            if (!validPassword(newAdmin.password)) {
                cout << "Password must contain at least one digit and one uppercase letter. Try again!" << n;
                continue;
            }
            break;
        } while (true);

        // Шифрування логіна та пароля методом Цезаря
        int shift = 3;
        newAdmin.login = encryptCaesar(newAdmin.login, shift);
        newAdmin.password = encryptCaesar(newAdmin.password, shift);

        admin = newAdmin; // Зберігаємо адміністратора у пам'яті
        isAdminSet = true; // Встановлюємо прапорець, що адміністратор встановлений
        cout << "Admin registration completed successfully!" << n;

        saveAdminToFile(); // Зберігаємо адміністратора у файл
    }

    // Вхід користувача в систему
    void userLogin() {
        string enteredLogin, enteredPassword;
        cout << "Enter login: ";
        cin >> enteredLogin;
        cout << "Enter password: ";
        cin >> enteredPassword;

        // Шифрування введеного логіна та пароля методом Цезаря
        int shift = 3;
        enteredLogin = encryptCaesar(enteredLogin, shift);
        enteredPassword = encryptCaesar(enteredPassword, shift);

        // Пошук користувача за введеним логіном
        User* user = searchUser(enteredLogin);

        // Перевірка введених даних з даними користувача
        if (enteredLogin == user->login && enteredPassword == user->password) {
            cout << "Login successful! Welcome, " << user->name << "!\n";
            currentUser = user; // Встановлюємо поточного користувача
            userMenu(); // Показуємо меню користувача
        }
        else {
            cout << "Login failed! Invalid login or password.\n";
        }
    }

    // Меню користувача після входу в систему
    void userMenu() {
        int choice;
        do {
            cout << n << "User Menu:" << n;
            cout << "1. Take a test" << n;
            cout << "2. View previous results" << n;
            cout << "3. Logout" << n;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                takeTest(); // Розпочати проходження тесту
                break;
            case 2:
                viewUserResults(); // Перегляд попередніх результатів користувача
                break;
            case 3:
                cout << "Logging out." << n;
                currentUser = nullptr; // Закінчення сеансу користувача
                return;
            default:
                cout << "Invalid choice. Please enter again." << n;
                break;
            }
        } while (true);
    }

    // Пошук тесту за назвою у файлі
    Test* searchTestFromFile(const string& testName) {
        ifstream inFile("saved_tests.txt");

        if (!inFile.is_open()) {
            cout << "Unable to open 'saved_tests.txt' for reading.\n";
            return nullptr;
        }

        string line;
        Test* foundTest = nullptr;
        while (getline(inFile, line)) {
            if (line.find("Test Name: " + testName) != string::npos) {
                foundTest = new Test();
                foundTest->name = testName;
                getline(inFile, line);
                while (getline(inFile, line) && line != "") {
                    if (line.substr(0, 9) == "Question ") {
                        foundTest->questions.push_back(line);
                        getline(inFile, line);
                        vector<string> choices;
                        for (int i = 0; i < 4; ++i) {
                            getline(inFile, line);
                            choices.push_back(line.substr(3));
                        }
                        foundTest->answer_choices.push_back(choices);
                        getline(inFile, line);
                        int correctIndex = stoi(line.substr(14)); 
                        foundTest->correct_answers_index.push_back(correctIndex);
                    }
                }
                break;
            }
        }

        inFile.close();
        return foundTest;
    }

    // Проходження тесту користувачем
    void takeTest() {
        if (!currentUser) {
            cout << "No user logged in." << n;
            return;
        }

        if (tests.empty()) {
            cout << "No categories available." << n;
            return;
        }

        // Вибір категорії тестів
        vector<string> categories;
        for (const auto& test : tests) {
            bool found = false;
            for (const auto& category : categories) {
                if (category == test.category) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories.push_back(test.category);
            }
        }

        cout << "Choose a category:" << n;
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i + 1 << ". " << categories[i] << n;
        }
        int categoryChoice;
        cout << "Enter your choice: ";
        cin >> categoryChoice;

        string chosenCategory = categories[categoryChoice - 1];

        // Вибір конкретного тесту у вибраній категорії
        vector<Test> testsInCategory;
        for (const auto& test : tests) {
            if (test.category == chosenCategory) {
                testsInCategory.push_back(test);
            }
        }

        cout << "Choose a test:" << n;
        for (size_t i = 0; i < testsInCategory.size(); ++i) {
            cout << i + 1 << ". " << testsInCategory[i].name << n;
        }
        int testChoice;
        cout << "Enter your choice: ";
        cin >> testChoice;

        Test* chosenTest = &testsInCategory[testChoice - 1];

        int correctCount = 0;
        // Проходження кожного питання тесту
        for (size_t i = 0; i < chosenTest->questions.size(); ++i) {
            cout << "Question " << i + 1 << ": " << chosenTest->questions[i] << "\n";
            for (size_t j = 0; j < chosenTest->answer_choices[i].size(); ++j) {
                cout << "  " << j + 1 << ". " << chosenTest->answer_choices[i][j] << "\n";
            }
            int answer;
            cout << "Enter your answer (1-4): ";
            cin >> answer;
            if (answer - 1 == chosenTest->correct_answers_index[i]) {
                ++correctCount;
            }
        }

        // Обчислення результатів тесту
        double percentageCorrect = (static_cast<double>(correctCount) / chosenTest->questions.size()) * 100;
        int score = static_cast<int>(percentageCorrect / 100 * 12);

        // Формування результатів тесту для виведення користувачу
        string result = "Test: " + chosenTest->name + "\n";
        result += "Number of correct answers: " + to_string(correctCount) + "\n";
        result += "Percentage of correct answers: " + to_string(percentageCorrect) + "%\n";
        result += "Score (out of 12): " + to_string(score) + "\n";

        cout << "Test completed.\n";
        cout << result << "\n";

        // Збереження результатів користувача
        user_results[currentUser->login].push_back(result);
        saveStatisticsToFile(user_results);
    }

    // Збереження статистики у файл
    void saveStatisticsToFile(const unordered_map<string, vector<string>>& user_results) {
        ofstream outFile("statistics.txt", ios::app);  // Відкриття файлу у режимі додавання
        if (outFile.is_open()) {
            for (const auto& entry : user_results) {
                outFile << "User: " << entry.first << "\n";
                for (const string& result : entry.second) {
                    outFile << result << "\n";
                }
                outFile << "\n"; // Роздільник між користувачами
            }
            outFile.close();
            cout << "Statistics saved successfully.\n";
        }
        else {
            cout << "Unable to open statistics file.\n";
        }
    }

    // Завантаження статистики з файлу
    void loadStatisticsFromFile() {
        ifstream inFile("statistics.txt");
        if (inFile.is_open()) {
            string line, user;
            while (getline(inFile, line)) {
                if (line.find("User: ") == 0) {
                    user = line.substr(6);
                    user_results[user] = vector<string>();
                }
                else if (!line.empty()) {
                    user_results[user].push_back(line);
                }
            }
            inFile.close();
        }
        else {
            cout << "Unable to open statistics file.\n";
        }
    }

    // Перегляд результатів користувача
    void viewUserResults() {
        if (!currentUser) {
            cout << "No user logged in." << "\n";
            return;
        }

        loadStatisticsFromFile();  // Завантаження статистики перед відображенням

        cout << "Results for user: " << currentUser->name << "\n";
        const auto& results = user_results[currentUser->login];
        for (const auto& result : results) {
            cout << "  - " << result << "\n";
        }
    }

    // Перегляд результатів всіх користувачів
    void viewResultsOfAllUsers(const unordered_map<string, vector<string>>& user_results) {
        ifstream inFile("statistics.txt");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                // Зчитуємо ім'я користувача
                if (line.find("User: ") != string::npos) {
                    string username = line.substr(6); // Отримуємо ім'я користувача
                    cout << "User: " << username << "\n";

                    // Виводимо результати тестів користувача
                    while (getline(inFile, line) && !line.empty()) {
                        cout << line << "\n";
                    }
                    cout << "\n"; // Роздільник між користувачами
                }
            }
            inFile.close();
        }
        else {
            cout << "Unable to open statistics file.\n";
        }
    }

    // Функція для входу адміністратора
    void adminLogin() {
        if (!isAdminSet) {
            cout << "Admin is not registered yet. Please register admin first." << n;
            return;
        }

        string login, password;
        cout << "Enter admin login: ";
        cin >> login;
        cout << "Enter admin password: ";
        cin >> password;

        int shift = 3;
        login = encryptCaesar(login, shift);
        password = encryptCaesar(password, shift);

        if (login == admin.login && password == admin.password) {
            cout << "Admin login successful!" << n;
            adminMenu();  // Перехід до меню адміністратора
        }
        else {
            cout << "Invalid admin credentials. Please try again." << n;
        }
    }

    // Функція для редагування тесту
    void editTest() {
        string testName;
        cout << "Enter the name of the test you want to edit: ";
        cin >> testName;

        Test* testToEdit = searchTest(testName);
        if (testToEdit == nullptr) {
            cout << "Test with name '" << testName << "' not found." << n;
            return;
        }

        int choice;
        do {
            cout << n << "Edit Test - " << testToEdit->name << ":" << n;
            cout << "1. Add a question" << n;
            cout << "2. Remove a question" << n;
            cout << "3. Exit" << n;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string question;
                cout << "Enter the question you want to add: ";
                cin.ignore();
                getline(cin, question);

                vector<string> choices;
                for (int j = 0; j < 4; ++j) {
                    string choice;
                    cout << "Enter choice " << j + 1 << ": ";
                    getline(cin, choice);
                    choices.push_back(choice);
                }

                int correctIndex;
                cout << "Enter index of correct choice (1-4): ";
                cin >> correctIndex;
                testToEdit->addQuestion(question, choices, correctIndex - 1);

                cout << "Question added successfully!" << n;
                break;
            }
            case 2: {
                int index;
                cout << "Enter the index of the question you want to remove (1-" << testToEdit->questions.size() << "): ";
                cin >> index;
                if (index < 1 || index > testToEdit->questions.size()) {
                    cout << "Invalid index. Please try again." << n;
                    break;
                }
                testToEdit->questions.erase(testToEdit->questions.begin() + index - 1);
                testToEdit->answer_choices.erase(testToEdit->answer_choices.begin() + index - 1);
                testToEdit->correct_answers_index.erase(testToEdit->correct_answers_index.begin() + index - 1);

                cout << "Question removed successfully!" << n;
                break;
            }
            case 3:
                cout << "Exiting edit menu." << n;
                return;
            default:
                cout << "Invalid choice. Please enter again." << n;
                break;
            }
        } while (true);
        saveTestsToFile(); //Збереження у файл
    }

    // Функція для додавання користувача адміністратором
    void addUserByAdmin() {
        if (!isAdminSet) {
            cout << "Admin is not registered yet. Please register admin first." << n;
            return;
        }

        User newUser;
        do {
            cout << "Enter the name of the user: ";
            cin >> newUser.name;
            if (any_of(newUser.name.begin(), newUser.name.end(), ::isdigit)) {
                cout << "The user's name cannot contain numbers! Please, try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the location of the user: ";
            cin >> newUser.location;
            if (any_of(newUser.location.begin(), newUser.location.end(), ::isdigit)) {
                cout << "The location name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the street of the user: ";
            cin >> newUser.street;
            if (any_of(newUser.street.begin(), newUser.street.end(), ::isdigit)) {
                cout << "The street name must contain only letters! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the house number of the user: ";
            cin >> newUser.house_number;
            if (none_of(newUser.house_number.begin(), newUser.house_number.end(), ::isdigit)) {
                cout << "The house number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the phone number of the user: ";
            cin >> newUser.num;
            if (none_of(newUser.num.begin(), newUser.num.end(), ::isdigit)) {
                cout << "The phone number must contain only numbers! Try again!" << n;
                continue;
            }
            break;
        } while (true);

        do {
            cout << "Enter the login of the user: ";
            cin >> newUser.login;
            if (searchUser(newUser.login)) {
                cout << "This login is already taken. Please, try another one.\n";
                continue;
            }
            cout << "Enter the password of the user: ";
            cin >> newUser.password;
            if (!validPassword(newUser.password)) {
                cout << "Password must contain at least one digit and one uppercase letter. Try again!" << endl;
                continue;
            }
            break;
        } while (true);

        int shift = 3;
        newUser.login = encryptCaesar(newUser.login, shift);
        newUser.password = encryptCaesar(newUser.password, shift);

        users.push_back(newUser);
        cout << "User added successfully!" << n;
        saveUsersToFile();  // Зберігаємо користувача у файл
    }

    // Функція для видалення користувача адміністратором
    void deleteUserByAdmin() {
        if (!isAdminSet) {
            cout << "Admin is not registered yet. Please register admin first." << n;
            return;
        }

        string login;
        cout << "Enter the login of the user you want to delete: ";
        cin >> login;

        // Видалення користувача з вектора за логіном
        auto it = std::remove_if(users.begin(), users.end(), [&](const User& user) { return user.login == login; });
        if (it != users.end()) {
            users.erase(it, users.end());
            cout << "User with login '" << login << "' deleted successfully." << n;
            saveUsersToFile();  // Зберігаємо оновлений список користувачів у файл
        }
        else {
            cout << "User with login '" << login << "' not found." << n;
        }
    }

    // Функція для модифікації користувача адміністратором
    void modifyUserByAdmin() {
        if (!isAdminSet) {
            cout << "Admin is not registered yet. Please register admin first." << n;
            return;
        }

        string login;
        cout << "Enter the login of the user you want to modify: ";
        cin >> login;

        // Пошук користувача за логіном
        User* userToModify = searchUser(login);
        if (userToModify) {
            cout << "User found. Enter new details:" << n;

            // Введення нових даних про користувача
            cout << "Enter the name of the user: ";
            cin >> userToModify->name;
            cout << "Enter the location of the user: ";
            cin >> userToModify->location;
            cout << "Enter the street of the user: ";
            cin >> userToModify->street;
            cout << "Enter the house number of the user: ";
            cin >> userToModify->house_number;
            cout << "Enter the phone number of the user: ";
            cin >> userToModify->num;
            cout << "Enter the login of the user: ";
            cin >> userToModify->login;
            cout << "Enter the password of the user: ";
            cin >> userToModify->password;

            // Шифрування логіну та паролю користувача
            int shift = 3;
            userToModify->login = encryptCaesar(userToModify->login, shift);
            userToModify->password = encryptCaesar(userToModify->password, shift);

            cout << "User details modified successfully!" << n;
            saveUsersToFile();  // Зберігаємо оновлені дані користувача у файл
        }
        else {
            cout << "User with login '" << login << "' not found." << n;
        }
    }



    void loadStatisticsFromFile(
        unordered_map<string, unordered_map<string, int>>& categoryScores,
        unordered_map<string, unordered_map<string, int>>& testScores,
        unordered_map<string, unordered_map<string, int>>& userTestScores
    ) {
        ifstream inFile("statistics.txt"); // Відкриття файлу для читання
        if (inFile.is_open()) { // Перевірка, чи вдалося відкрити файл
            string line;
            string currentUser;

            while (getline(inFile, line)) { // Читання рядка з файлу
                if (line.find("User: ") == 0) { // Перевірка, чи рядок містить інформацію про користувача
                    currentUser = line.substr(6); // Отримання імені користувача з рядка
                }
                else if (line.find("Test: ") == 0) { // Перевірка, чи рядок містить інформацію про тест
                    string test = line.substr(6); // Отримання назви тесту з рядка
                    int score = 0;

                    // Пропуск рядка "Number of correct answers"
                    getline(inFile, line);

                    // Пропуск рядка "Percentage of correct answers"
                    getline(inFile, line);

                    // Отримання балу тесту з рядка "Score (out of X)"
                    getline(inFile, line);
                    size_t pos1 = line.find(": ");
                    if (pos1 != string::npos) {
                        score = stoi(line.substr(pos1 + 2));
                    }

                    // Додавання балу до загальних результатів тесту
                    testScores[test][currentUser] += score;
                    userTestScores[currentUser][test] += score;
                }
            }
            inFile.close();
        }
        else {
            cout << "Unable to open statistics file.\n";
        }
    }



    void viewOverallTestResults() {
        unordered_map<string, unordered_map<string, int>> categoryScores; // Структура для зберігання результатів за категоріями
        unordered_map<string, unordered_map<string, int>> testScores; // Структура для зберігання результатів за тестами
        unordered_map<string, unordered_map<string, int>> userTestScores; // Структура для зберігання результатів за користувачами

        loadStatisticsFromFile(categoryScores, testScores, userTestScores); // Завантаження статистики з файлу

        cout << "Select the type of results to view:\n";
        cout << "1. Overall results by category\n";
        cout << "2. Overall results by test\n";
        cout << "3. Overall results by user\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        // Виведення меню вибору типу результатів для перегляду
        switch (choice) {
        case 1: { // Виведення результатів за категорією
            cout << "Enter the category to view: ";
            string category;
            cin.ignore(); // Очистка буфера після вводу числа
            getline(cin, category);
            cout << "\nOverall Test Results for Category: " << category << "\n";
            if (categoryScores.find(category) != categoryScores.end()) {
                for (const auto& userEntry : categoryScores[category]) {
                    cout << "  User: " << userEntry.first << ", Total Score: " << userEntry.second << "\n";
                }
            }
            else {
                cout << "No results found for this category.\n";
            }
            break;
        }
        case 2: { // Виведення результатів за тестом
            cout << "Enter the test name to view: ";
            string test;
            cin.ignore(); // Очистка буфера після вводу числа
            getline(cin, test);
            cout << "\nOverall Test Results for Test: " << test << "\n";
            if (testScores.find(test) != testScores.end()) {
                for (const auto& userEntry : testScores[test]) {
                    cout << "  User: " << userEntry.first << ", Total Score: " << userEntry.second << "\n";
                }
            }
            else {
                cout << "No results found for this test.\n";
            }
            break;
        }
        case 3: { // Виведення результатів за користувачем
            cout << "Enter the user login to view: ";
            string user;
            cin.ignore(); // Очистка буфера після вводу числа
            getline(cin, user);
            cout << "\nOverall Test Results for User: " << user << "\n";
            if (userTestScores.find(user) != userTestScores.end()) {
                for (const auto& testEntry : userTestScores[user]) {
                    cout << "  Test: " << testEntry.first << ", Score: " << testEntry.second << "\n";
                }
            }
            else {
                cout << "No results found for this user.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }
    }

    void adminMenu() {
        int choice;
        do {
            cout << n << "Admin Menu:" << n;
            cout << "1. Add a test" << n;
            cout << "2. View all tests" << n;
            cout << "3. View results of all users" << n;
            cout << "4. Edit a test" << n;
            cout << "5. Add a user" << n;
            cout << "6. Delete user" << n;
            cout << "7. Modify user" << n;
            cout << "8. View overall test results" << n;
            cout << "9. Exit" << n;
            cout << "Enter your choice: ";
            cin >> choice; // Вибір опції з меню адміністратора

            switch (choice) {
            case 1:
                addTest(); // Додавання нового тесту
                break;
            case 2:
                viewAllTests(); // Перегляд усіх тестів
                break;
            case 3:
                viewAllResults(user_results); // Перегляд результатів всіх користувачів
                break;
            case 4:
                editTest(); // Редагування існуючого тесту
                break;
            case 5:
                addUserByAdmin(); // Додавання нового користувача адміністратором
                break;
            case 6:
                deleteUserByAdmin(); // Видалення користувача адміністратором
                break;
            case 7:
                modifyUserByAdmin(); // Редагування інформації про користувача адміністратором
                break;
            case 8:
                viewOverallTestResults(); // Перегляд загальних результатів тестів
                break;
            case 9:
                cout << "Exiting admin menu." << n;
                saveTestsToFile(); // Збереження тестів до файлу перед виходом
                return;
            default:
                cout << "Invalid choice. Please enter again." << n;
                break;
            }
        } while (true);
    }

    void addTest() {
        string category, testName;
        cout << "Enter category of the test: ";
        cin >> category;
        cout << "Enter name of the test: ";
        cin >> testName;

        Test newTest(category, testName); // Створення нового об'єкту тесту
        int numQuestions;
        cout << "Enter number of questions for the test: ";
        cin >> numQuestions;

        for (int i = 0; i < numQuestions; ++i) {
            string question;
            cout << "Enter question " << i + 1 << ": ";
            cin.ignore();  // Очищення буфера перед введенням текстового рядка
            getline(cin, question);

            vector<string> choices;
            for (int j = 0; j < 4; ++j) {
                string choice;
                cout << "Enter choice " << j + 1 << ": ";
                getline(cin, choice);
                choices.push_back(choice);
            }

            int correctIndex;
            cout << "Enter index of correct choice (1-4): ";
            cin >> correctIndex;
            newTest.addQuestion(question, choices, correctIndex - 1); 
        }

        tests.push_back(newTest); // Додавання нового тесту до загального списку тестів
        cout << "Test added successfully!" << n;

        saveTestsToFile();
    }

    void writeTestsToFile(const string& fileName) {
        ofstream outFile(fileName);  // Відкриття файлу для запису

        if (outFile.is_open()) {
            for (const auto& test : tests) {
                outFile << "Category: " << test.category << "\n";
                outFile << "Test Name: " << test.name << "\n";
                outFile << "Questions:\n";
                for (size_t i = 0; i < test.questions.size(); ++i) {
                    outFile << "Question " << i + 1 << ": " << test.questions[i] << "\n";
                    outFile << "Choices:\n";
                    for (size_t j = 0; j < test.answer_choices[i].size(); ++j) {
                        outFile << j + 1 << ". " << test.answer_choices[i][j] << "\n";
                    }
                    outFile << "Correct Index: " << test.correct_answers_index[i] << "\n";
                }
                outFile << "\n";
            }
            outFile.close();
            cout << "Tests have been written to '" << fileName << "'.\n";
        }
        else {
            cout << "Unable to open file '" << fileName << "' for writing.\n";
        }
    }

    void viewAllTests() {
        cout << "All Tests:" << n;
        for (const Test& test : tests) { // Виведення усіх тестів
            cout << "Category: " << test.category << ", Test Name: " << test.name << n;
            for (size_t i = 0; i < test.questions.size(); ++i) {
                cout << "Question " << i + 1 << ": " << test.questions[i] << n;
                for (size_t j = 0; j < test.answer_choices[i].size(); ++j) {
                    cout << "  " << j + 1 << ". " << test.answer_choices[i][j] << n;
                }
            }
            cout << n;
        }
    }

    void loadStatisticsFromFile(unordered_map<string, vector<string>>& user_results) {
        ifstream inFile("statistics.txt"); // Відкриття файлу для читання
        if (inFile.is_open()) {
            string line;
            string currentUser;
            while (getline(inFile, line)) {
                if (line.find("User: ") != string::npos) {
                    currentUser = line.substr(6); // Отримуємо ім'я користувача
                    continue;
                }
                user_results[currentUser].push_back(line); // Додаємо результати для поточного користувача
            }
            inFile.close();
            cout << "Statistics loaded from file.\n";
        }
        else {
            cout << "Unable to open statistics file.\n";
        }
    }


    void viewAllResults(unordered_map<string, vector<string>>& user_results) {
        cout << "Results of All Users:" << endl;

        // Завантаження статистики з файлу
        loadStatisticsFromFile(user_results);

        // Виведення статистики на екран
        for (const auto& entry : user_results) {
            cout << "User: " << entry.first << endl;
            for (const string& result : entry.second) {
                cout << "  - " << result << endl;
            }
            cout << endl;
        }
    }


    void showUserResults() {
    if (!currentUser) { // Перевірка, чи користувач залогінений
        cout << "No user logged in." << n;
        return;
    }

    if (user_results.find(currentUser->login) == user_results.end()) { // Перевірка, чи є результати для поточного користувача
        cout << "No results available for this user." << n;
        return;
    }

    cout << "Results for " << currentUser->login << ":" << n;
    for (const auto& result : user_results[currentUser->login]) { // Виведення результатів для поточного користувача
        cout << result << n;
    }
}

    void start() {
        int choice;
        do {
            cout << n << "Main Menu:" << n;
            cout << "1. Register as a new user" << n;
            cout << "2. Register as an admin" << n;
            cout << "3. Admin login" << n;
            cout << "4. User login" << n;
            cout << "5. Exit" << n;
            cout << "Enter your choice: ";
            cin >> choice; // Вибір опції з головного меню програми

            switch (choice) {
            case 1:
                registerUser(); // Реєстрація нового користувача
                break;
            case 2:
                registerAdmin(); // Реєстрація нового адміністратора
                break;
            case 3:
                adminLogin(); // Логін адміністратора
                break;
            case 4:
                userLogin(); // Логін користувача
                break;
            case 5:
                cout << "Exiting program." << n;
                return;
            default:
                cout << "Invalid choice. Please enter again." << n;
                break;
            }
        } while (true);
    }
};