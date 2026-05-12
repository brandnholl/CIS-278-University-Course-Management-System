// Student.h
#pragma once
#include <string>
#include <set>
#include <iostream>
using namespace std;

struct Student {
    string id;
    string name;
    set<string> courses; // set prevents duplicate course entries

    Student() {}
    Student(string id, string name) : id(id), name(name) {}

    void print() {
        cout << "  ID   : " << id << "\n";
        cout << "  Name : " << name << "\n";
        cout << "  Courses: ";
        if (courses.empty()) { cout << "none\n"; return; }
        for (string c : courses) cout << c << " ";
        cout << "\n";
    }
};
