// main.cpp - University Course Management System
#include <iostream>
#include <map>
#include <deque>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#include "Student.h"
#include "Course.h"

// -- Global registries --
map<string, Student> students;    // key = student ID
map<string, Course>  courses;     // key = course code
deque<string>        activityLog; // log with fast push/pop on both ends

// -------------------------------------------------------
// Utility
// -------------------------------------------------------

// Append to log, keep max 100 entries
void addLog(string msg) {
    activityLog.push_back(msg);
    if (activityLog.size() > 100)
        activityLog.pop_front();
}

string getInput(string prompt) {
    cout << "  " << prompt << ": ";
    string val;
    getline(cin, val);
    return val;
}

// -------------------------------------------------------
// Student Management
// -------------------------------------------------------

void addStudent() {
    string id   = getInput("Student ID");
    string name = getInput("Full Name");
    if (students.count(id)) { cout << "  ID already exists.\n"; return; }
    students[id] = Student(id, name);
    addLog("ADD student " + id + " - " + name);
    cout << "  Student added.\n";
}

void removeStudent() {
    string id = getInput("Student ID");
    if (!students.count(id)) { cout << "  Not found.\n"; return; }
    // drop student from all their courses
    for (string code : students[id].courses)
        if (courses.count(code)) courses[code].students.erase(id);
    students.erase(id);
    addLog("REMOVE student " + id);
    cout << "  Student removed.\n";
}

void updateStudent() {
    string id = getInput("Student ID");
    if (!students.count(id)) { cout << "  Not found.\n"; return; }
    string newName = getInput("New name");
    students[id].name = newName;
    addLog("UPDATE student " + id + " -> " + newName);
    cout << "  Updated.\n";
}

void viewStudent() {
    string id = getInput("Student ID");
    if (!students.count(id)) { cout << "  Not found.\n"; return; }
    students[id].print();
}

void listStudents() {
    if (students.empty()) { cout << "  No students.\n"; return; }
    for (pair<string,Student> e : students) { cout << "\n"; e.second.print(); }
}

void searchStudents() {
    string kw = getInput("Name keyword");
    // lowercase keyword for case-insensitive match
    for (int i = 0; i < (int)kw.size(); i++) kw[i] = tolower(kw[i]);
    bool found = false;
    for (pair<string,Student> e : students) {
        string lower = e.second.name;
        for (int i = 0; i < (int)lower.size(); i++) lower[i] = tolower(lower[i]);
        if (lower.find(kw) != string::npos) {
            cout << "  " << e.first << " - " << e.second.name << "\n";
            found = true;
        }
    }
    if (!found) cout << "  No matches.\n";
}

// -------------------------------------------------------
// Course Management
// -------------------------------------------------------

void addCourse() {
    string code  = getInput("Course Code");
    string title = getInput("Title");
    string instr = getInput("Instructor");
    string capStr = getInput("Capacity [30]");
    int cap = 30;
    if (!capStr.empty()) {
        try { cap = stoi(capStr); }
        catch (...) { cout << "  Invalid number, using 30.\n"; cap = 30; }
    }
    if (courses.count(code)) { cout << "  Code already exists.\n"; return; }
    courses[code] = Course(code, title, instr, cap);
    addLog("ADD course " + code + " - " + title);
    cout << "  Course added.\n";
}

void removeCourse() {
    string code = getInput("Course Code");
    if (!courses.count(code)) { cout << "  Not found.\n"; return; }
    // remove course from all enrolled students
    for (string sid : courses[code].students)
        if (students.count(sid)) students[sid].courses.erase(code);
    courses.erase(code);
    addLog("REMOVE course " + code);
    cout << "  Course removed.\n";
}

void updateCourse() {
    string code = getInput("Course Code");
    if (!courses.count(code)) { cout << "  Not found.\n"; return; }
    courses[code].title      = getInput("New Title");
    courses[code].instructor = getInput("New Instructor");
    string capStr = getInput("New Capacity [" + to_string(courses[code].capacity) + "]");
    if (!capStr.empty()) {
        try { courses[code].capacity = stoi(capStr); }
        catch (...) { cout << "  Invalid number, capacity unchanged.\n"; }
    }
    addLog("UPDATE course " + code);
    cout << "  Updated.\n";
}

void viewCourse() {
    string code = getInput("Course Code");
    if (!courses.count(code)) { cout << "  Not found.\n"; return; }
    courses[code].print();
}

void listCourses() {
    if (courses.empty()) { cout << "  No courses.\n"; return; }
    for (pair<string,Course> e : courses) { cout << "\n"; e.second.print(); }
}

void searchCourses() {
    string kw = getInput("Title or instructor keyword");
    for (int i = 0; i < (int)kw.size(); i++) kw[i] = tolower(kw[i]);
    bool found = false;
    for (pair<string,Course> e : courses) {
        string lt = e.second.title, li = e.second.instructor;
        for (int i = 0; i < (int)lt.size(); i++) lt[i] = tolower(lt[i]);
        for (int i = 0; i < (int)li.size(); i++) li[i] = tolower(li[i]);
        if (lt.find(kw) != string::npos || li.find(kw) != string::npos) {
            cout << "  " << e.first << " - " << e.second.title << "\n";
            found = true;
        }
    }
    if (!found) cout << "  No matches.\n";
}

// -------------------------------------------------------
// Enrollment
// -------------------------------------------------------

void enrollStudent() {
    string sid  = getInput("Student ID");
    string code = getInput("Course Code");
    if (!students.count(sid))  { cout << "  Student not found.\n"; return; }
    if (!courses.count(code))  { cout << "  Course not found.\n";  return; }
    if (students[sid].courses.count(code)) { cout << "  Already enrolled.\n"; return; }

    if (courses[code].isFull()) {
        courses[code].waitlist.push(sid);
        addLog("WAITLIST " + sid + " -> " + code);
        cout << "  Course full. Added to waitlist.\n";
        return;
    }

    // update both sides of the relationship
    students[sid].courses.insert(code);
    courses[code].students.insert(sid);
    addLog("ENROLL " + sid + " -> " + code);
    cout << "  Enrolled.\n";
}

void dropStudent() {
    string sid  = getInput("Student ID");
    string code = getInput("Course Code");
    if (!students.count(sid))               { cout << "  Student not found.\n";      return; }
    if (!courses.count(code))               { cout << "  Course not found.\n";       return; }
    
    
    if (!students[sid].courses.count(code)) { 
        queue<string> rebuilt;
        bool found = false;
        while (!courses[code].waitlist.empty()) {
            string front = courses[code].waitlist.front();
            courses[code].waitlist.pop();
            if (front == sid) { found = true; }
            else { rebuilt.push(front); }
        }
        courses[code].waitlist = rebuilt;
        if (found) {
            addLog("REMOVE WAITLIST " + sid + " from " + code);
            cout << " Removed from waitlist.\n";
        } else {
            cout << " Not enrolled or waitlisted here.\n";
        }
        return;
    }

    students[sid].courses.erase(code);
    courses[code].students.erase(sid);
    addLog("DROP " + sid + " from " + code);
    cout << "  Dropped.\n";

    // promote next waitlisted student if any
    if (!courses[code].waitlist.empty()) {
        string next = courses[code].waitlist.front();
        courses[code].waitlist.pop();
        if (students.count(next)) {
            students[next].courses.insert(code);
            courses[code].students.insert(next);
            addLog("PROMOTE " + next + " -> " + code);
            cout << "  " << next << " promoted from waitlist.\n";
        }
    }
}

// -------------------------------------------------------
// Admin Tools
// -------------------------------------------------------

// Uses pair to bundle code + title for display
void coursesForStudent() {
    string sid = getInput("Student ID");
    if (!students.count(sid)) { cout << "  Not found.\n"; return; }
    if (students[sid].courses.empty()) { cout << "  Not enrolled anywhere.\n"; return; }
    cout << "\n  Courses for " << students[sid].name << ":\n";
    for (string code : students[sid].courses) {
        pair<string,string> entry;
        entry.first  = code;
        entry.second = courses.count(code) ? courses[code].title : "unknown";
        cout << "  " << entry.first << " - " << entry.second << "\n";
    }
}

// Uses pair to bundle id + name for display
void studentsInCourse() {
    string code = getInput("Course Code");
    if (!courses.count(code)) { cout << "  Not found.\n"; return; }
    if (courses[code].students.empty()) { cout << "  No students enrolled.\n"; return; }
    cout << "\n  Students in " << courses[code].title << ":\n";
    for (string sid : courses[code].students) {
        pair<string,string> entry;
        entry.first  = sid;
        entry.second = students.count(sid) ? students[sid].name : "unknown";
        cout << "  " << entry.first << " - " << entry.second << "\n";
    }
}

void printLog() {
    if (activityLog.empty()) { cout << "  Log is empty.\n"; return; }
    cout << "\n  Activity Log:\n";
    for (int i = 0; i < (int)activityLog.size(); i++)
        cout << "  " << activityLog[i] << "\n";
}

// -------------------------------------------------------
// Save / Load
// -------------------------------------------------------

void saveData() {
    ofstream sf("students.csv");
    sf << "ID,Name,Courses\n";
    for (pair<string,Student> e : students) {
        sf << e.second.id << "," << e.second.name << ",";
        bool first = true;
        for (string c : e.second.courses) { if (!first) sf << "|"; sf << c; first = false; }
        sf << "\n";
    }

    ofstream cf("courses.csv");
    cf << "Code,Title,Instructor,Capacity,Students\n";
    for (pair<string,Course> e : courses) {
        cf << e.second.code << "," << e.second.title << ","
           << e.second.instructor << "," << e.second.capacity << ",";
        bool first = true;
        for (string s : e.second.students) { if (!first) cf << "|"; cf << s; first = false; }
        cf << "\n";
    }
    cout << "  Saved to students.csv and courses.csv\n";
    addLog("SAVE to CSV");
}

void loadData() {
    students.clear();
    courses.clear();

    // load courses first so enroll references exist
    ifstream cf("courses.csv");
    if (!cf) { cout << "  Cannot open courses.csv\n"; return; }
    string line;
    getline(cf, line); // skip header
    while (getline(cf, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string code, title, instr, capStr, enrolled;
        getline(ss, code,    ',');
        getline(ss, title,   ',');
        getline(ss, instr,   ',');
        getline(ss, capStr,  ',');
        getline(ss, enrolled,',');
        int cap = capStr.empty() ? 30 : stoi(capStr);
        courses[code] = Course(code, title, instr, cap);
        istringstream es(enrolled);
        string sid;
        while (getline(es, sid, '|'))
            if (!sid.empty()) courses[code].students.insert(sid);
    }

    ifstream sf("students.csv");
    if (!sf) { cout << "  Cannot open students.csv\n"; return; }
    getline(sf, line); // skip header
    while (getline(sf, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, enrolled;
        getline(ss, id,      ',');
        getline(ss, name,    ',');
        getline(ss, enrolled,',');
        students[id] = Student(id, name);
        istringstream es(enrolled);
        string code;
        while (getline(es, code, '|'))
            if (!code.empty()) students[id].courses.insert(code);
    }

    cout << "  Loaded " << students.size() << " students, "
         << courses.size() << " courses.\n";
    addLog("LOAD from CSV");
}

// -------------------------------------------------------
// Menus
// -------------------------------------------------------

void studentMenu() {
    string c;
    while (true) {
        cout << "\n-- Student Management --\n"
             << "  1. Add\n  2. Remove\n  3. Update\n"
             << "  4. View\n  5. List all\n  6. Search\n  0. Back\n";
        c = getInput("Choice");
        if      (c == "1") addStudent();
        else if (c == "2") removeStudent();
        else if (c == "3") updateStudent();
        else if (c == "4") viewStudent();
        else if (c == "5") listStudents();
        else if (c == "6") searchStudents();
        else if (c == "0") break;
        else cout << "  Invalid.\n";
    }
}

void courseMenu() {
    string c;
    while (true) {
        cout << "\n-- Course Management --\n"
             << "  1. Add\n  2. Remove\n  3. Update\n"
             << "  4. View\n  5. List all\n  6. Search\n  0. Back\n";
        c = getInput("Choice");
        if      (c == "1") addCourse();
        else if (c == "2") removeCourse();
        else if (c == "3") updateCourse();
        else if (c == "4") viewCourse();
        else if (c == "5") listCourses();
        else if (c == "6") searchCourses();
        else if (c == "0") break;
        else cout << "  Invalid.\n";
    }
}

void enrollmentMenu() {
    string c;
    while (true) {
        cout << "\n-- Enrollment --\n"
             << "  1. Enroll student\n  2. Drop student\n  0. Back\n";
        c = getInput("Choice");
        if      (c == "1") enrollStudent();
        else if (c == "2") dropStudent();
        else if (c == "0") break;
        else cout << "  Invalid.\n";
    }
}

void adminMenu() {
    string c;
    while (true) {
        cout << "\n-- Admin Tools --\n"
             << "  1. Courses for a student\n"
             << "  2. Students in a course\n"
             << "  3. Activity log\n"
             << "  0. Back\n";
        c = getInput("Choice");
        if      (c == "1") coursesForStudent();
        else if (c == "2") studentsInCourse();
        else if (c == "3") printLog();
        else if (c == "0") break;
        else cout << "  Invalid.\n";
    }
}

// -------------------------------------------------------
// Main
// -------------------------------------------------------

int main() {
    cout << "==============================\n";
    cout << "  Course Management System\n";
    cout << "==============================\n";

    string c;
    while (true) {
        cout << "\n=== Main Menu ===\n"
             << "  1. Student Management\n"
             << "  2. Course Management\n"
             << "  3. Enrollment\n"
             << "  4. Admin Tools\n"
             << "  5. Save\n"
             << "  6. Load\n"
             << "  0. Exit\n";
        c = getInput("Choice");
        if      (c == "1") studentMenu();
        else if (c == "2") courseMenu();
        else if (c == "3") enrollmentMenu();
        else if (c == "4") adminMenu();
        else if (c == "5") saveData();
        else if (c == "6") loadData();
        else if (c == "0") { cout << "Goodbye!\n"; break; }
        else cout << "  Invalid.\n";
    }
    return 0;
}
