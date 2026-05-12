# University Course Management System

A C++17 command-line program for managing students, courses, and enrollments.

## Files

```
Student.h       – Student struct
Course.h        – Course struct (includes waitlist)
main.cpp        – All logic and menus
Makefile        – Build instructions
students.csv    – Test student data
courses.csv     – Test course data
```

## Build & Run

```bash
make       # compiles to ./cms
./cms      # run the program
make clean # remove binary and CSV files
```

**Windows (MinGW):**
```bash
g++ -std=c++17 -Wall main.cpp -o cms.exe
```

## Menu Options

```
1. Student Management  – add, remove, update, view, list, search
2. Course Management   – add, remove, update, view, list, search
3. Enrollment          – enroll (auto-waitlists if full), drop (auto-promotes waitlist)
4. Admin Tools         – courses per student, students per course, activity log
5. Save                – writes students.csv and courses.csv
6. Load                – reads students.csv and courses.csv
```

## Loading Test Data

Place `students.csv` and `courses.csv` in the same folder as `./cms`,
then choose option **6 (Load)** from the main menu.

## CSV Format

**students.csv**
```
ID,Name,Courses
S001,Alice Johnson,CS101|MATH101
```

**courses.csv**
```
Code,Title,Instructor,Capacity,Students
CS101,Intro to Programming,Dr. Smith,3,S001|S002
```
- Courses / Students columns use `|` as a separator.
- Waitlists are not saved; re-enroll waitlisted students after a load.
