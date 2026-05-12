// Course.h
#pragma once
#include <string>
#include <set>
#include <queue>
#include <iostream>
using namespace std;

struct Course {
    string code;
    string title;
    string instructor;
    int capacity;
    set<string>   students; // enrolled students, no duplicates
    queue<string> waitlist; // FIFO queue for overflow

    Course() {}
    Course(string code, string title, string instructor, int capacity)
        : code(code), title(title), instructor(instructor), capacity(capacity) {}

    bool isFull() { return (int)students.size() >= capacity; }

    void print() {
        cout << "  Code       : " << code << "\n";
        cout << "  Title      : " << title << "\n";
        cout << "  Instructor : " << instructor << "\n";
        cout << "  Enrolled   : " << students.size() << " / " << capacity << "\n";
        cout << "  Waitlist   : " << waitlist.size() << "\n";
        cout << "  Students   : ";
        if (students.empty()) { cout << "none\n"; return; }
        for (string s : students) cout << s << " ";
        cout << "\n";
    }
};
