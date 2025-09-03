# 📘 CGPA Calculator (C++)

A **console-based CGPA Calculator** built in C++.  
It allows students to manage multiple semesters, add courses, calculate GPA/CGPA, search and sort courses, edit grades, and save/load results from files.

---

## ✨ Features
- **Multi-Semester Support** → Manage any number of semesters.
- **Course Management** → Add courses with name, credits, and grade.
- **Grade → Grade Point Conversion**  
  - Supports *O, A+, A, B+, B, C, P/E, F* (10-point scale).  
  - Fallback to simple *A–F* mapping.
- **Accurate GPA & CGPA Calculation** → Weighted by course credits.
- **Search Courses** → Case-insensitive search across semesters.
- **Sorting** → By course name, credits, or grade points (asc/desc).
- **Edit Grades** → Update a course grade and auto-recalculate.
- **Save & Load** → Store results in a human-readable `.txt` file.
- **Clear Data** → Reset record and start fresh.

---

## ⚙️ Technologies Used
- **C++17**
- **STL** (Vectors, Algorithms, String utilities)
- **File Handling** (`ifstream`, `ofstream`)
- **Formatted Output** (`iomanip` for tables)
