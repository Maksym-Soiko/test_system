#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
using namespace std;

// Клас для тестів
class Test {
public:
    string category;
    string name;
    vector<string> questions;
    vector<vector<string>> answer_choices;
    vector<int> correct_answers_index;

    Test() = default;

    Test(string category, string name)
        : category(category), name(name), questions(), answer_choices(), correct_answers_index() {}

    void addQuestion(const string& question, const vector<string>& choices, int correctIndex) {
        questions.push_back(question);
        answer_choices.push_back(choices);
        correct_answers_index.push_back(correctIndex);
    }
};
