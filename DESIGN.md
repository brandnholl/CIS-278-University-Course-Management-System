# Design Decisions & STL Usage

## Structure

Three files keep the code organised without being over-engineered:

- **Student.h** – plain `struct` holding a student's ID, name, and enrolled course codes.
- **Course.h** – plain `struct` holding course info, an enrolled-student set, and a waitlist queue.
- **main.cpp** – all business logic written as free functions, plus menus and file I/O.

Using `struct` instead of `class` keeps things readable for a beginner — members are
public by default, and there is no need for getters or setters. `using namespace std`
removes repetitive `std::` prefixes throughout.

---

## STL Container Choices

### `std::map` – student and course registries
Both `students` and `courses` are `map<string, T>` keyed on ID / course code.
- Lookup, insert, and delete are all O(log n).
- Iterating a map always goes in sorted key order, so listing students or courses
  comes out alphabetically for free.

### `std::set` – enrollment lists
`Student::courses` and `Course::students` are both `set<string>`.
- A set rejects duplicate insertions automatically, satisfying the "no duplicate
  enrollments" requirement without any extra if-check.
- Membership test (`set::count`) is O(log n).

### `std::queue` – waitlist
`Course::waitlist` is a `queue<string>` of student IDs.
- FIFO order means the student who joined the waitlist first gets promoted first —
  which is the expected fair behaviour.
- When a student drops, `queue::front()` + `queue::pop()` promotes the next
  candidate in O(1).

### `std::deque` – activity log
The global `activityLog` is a `deque<string>`.
- New events are appended with `push_back` in O(1).
- When the log exceeds 100 entries the oldest is removed with `pop_front` in O(1).
- A `vector` could not do the front-removal cheaply (O(n)); a `list` would lose
  random-index access. `deque` gives both.

### `std::pair` – search result display
`coursesForStudent` and `studentsInCourse` build a `pair<string,string>` for
each result to bundle the key (code / ID) with the display label (title / name)
before printing. This shows the pair concept explicitly without needing an
extra struct.

---

## Enrollment Logic

Enrollment touches **both sides** of the relationship in one step:

```
students[sid].courses.insert(code)   ← student knows their course
courses[code].students.insert(sid)   ← course knows its student
```

Drop reverses both, then checks the waitlist. If a waitlisted student is found,
they are promoted by repeating the same two-sided insert.

---

## Persistence

Save writes two plain CSV files (`students.csv`, `courses.csv`) using `|` as the
delimiter inside list columns. Load reads courses first so that student records
can reference valid course codes when being rebuilt. Waitlist state is runtime-only
and is not persisted.
