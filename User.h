#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "Person.h"
using namespace std;

// Клас користувача
class User : public Person {
public:
    string login; // логін
    string password; // пароль
    vector<string> test_results; // результати тестів

    User() = default;

    User(string name, string location, string street, string house_number, string num, string login, string password)
        : Person(name, location, street, house_number, num), login(login), password(password) {}
};