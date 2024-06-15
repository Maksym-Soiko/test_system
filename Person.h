#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <cstring>
#include <string>

using namespace std;
// Базовий клас для всіх користувачів
class Person {
public:
    string name;
    string location;
    string street;
    string house_number;
    string num; // номер телефону

    Person() = default; // використовувати генерований конструктор за замовченням

    Person(string name, string location, string street, string house_number, string num)
        : name(name), location(location), street(street), house_number(house_number), num(num) {}
};