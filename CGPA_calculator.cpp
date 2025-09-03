#include <bits/stdc++.h>
using namespace std;

struct Course {
    string name;
    double credits{};
    string gradeText;     // e.g., "A", "A+", "B", "O", "F"
    double gradePoint{};  // derived from gradeText
};

struct Semester {
    string title;                 // e.g., "Sem 1 (Jan 2025)"
    vector<Course> courses;

    static string toUpperStr(string s) {
        for (char &c : s) c = (char)toupper((unsigned char)c);
        return s;
    }

    static double gradeToPoint(const string &graw) {
        string g = toUpperStr(graw);
        // Common Indian 10-point variants first
        if (g == "O")  return 10.0;
        if (g == "A+") return 9.0;
        if (g == "A")  return 8.0;
        if (g == "B+") return 7.0;
        if (g == "B")  return 6.0;
        if (g == "C")  return 5.0;
        if (g == "P" || g == "E") return 4.0; // pass/effort (varies by institute)
        if (g == "F" || g == "I") return 0.0;

        // Simple fallback scale for single letters (A–F)
        if (g.size() == 1) {
            char ch = g[0];
            switch (ch) {
                case 'A': return 10.0;
                case 'B': return 8.0;
                case 'C': return 6.0;
                case 'D': return 5.0;
                case 'E': return 4.0;
                case 'F': return 0.0;
                default: break;
            }
        }
        return -1.0; // invalid/unrecognized
    }

    double totalCredits() const {
        double s = 0.0;
        for (const auto &c : courses) s += c.credits;
        return s;
    }

    double totalWeightedPoints() const {
        double s = 0.0;
        for (const auto &c : courses) s += c.credits * c.gradePoint;
        return s;
    }

    double gpa() const {
        double tc = totalCredits();
        if (tc <= 0) return 0.0;
        return totalWeightedPoints() / tc;
    }
};

struct StudentRecord {
    vector<Semester> semesters;

    double cgpa() const {
        double sumCredits = 0.0, sumWeighted = 0.0;
        for (const auto &sem : semesters) {
            sumCredits  += sem.totalCredits();
            sumWeighted += sem.totalWeightedPoints();
        }
        if (sumCredits <= 0) return 0.0;
        return sumWeighted / sumCredits;
    }

    void clear() { semesters.clear(); }
};

// ---------- Small helpers ----------
static void flushLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string inputLineTrimmed(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    // trim both ends
    auto notspace = [](int ch){ return !isspace(ch); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), notspace));
    s.erase(find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
    return s;
}

static double inputPositiveDouble(const string &prompt) {
    while (true) {
        cout << prompt;
        double x;
        if (cin >> x && x > 0) { flushLine(); return x; }
        cout << "  ! Enter a positive number.\n";
        flushLine();
    }
}

static int inputIntInRange(const string &prompt, int lo, int hi) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x && x >= lo && x <= hi) { flushLine(); return x; }
        cout << "  ! Enter an integer in [" << lo << ", " << hi << "].\n";
        flushLine();
    }
}

static string upper(const string &s) {
    string t = s;
    for (auto &c : t) c = (char)toupper((unsigned char)c);
    return t;
}

static bool icontains(const string &text, const string &needle) {
    string a = upper(text), b = upper(needle);
    return a.find(b) != string::npos;
}

// ---------- Core functionality ----------
static Course makeCourseInteractive() {
    Course c;
    while (true) {
        c.name = inputLineTrimmed("Enter course name: ");
        if (!c.name.empty()) break;
        cout << "  ! Name cannot be empty.\n";
    }
    c.credits = inputPositiveDouble("Enter credits (e.g., 3 or 3.5): ");

    while (true) {
        c.gradeText = upper(inputLineTrimmed(
            "Enter grade (O, A+, A, B+, B, C, P/E, F) or simple A–F: "));
        double gp = Semester::gradeToPoint(c.gradeText);
        if (gp >= 0.0) { c.gradePoint = gp; break; }
        cout << "  ! Invalid grade. Try again.\n";
    }
    return c;
}

static void addSemester(StudentRecord &rec) {
    Semester s;
    s.title = inputLineTrimmed("Semester title (e.g., Sem 1 / Autumn 2025): ");
    int n = inputIntInRange("How many courses in this semester? ", 1, 1000);
    for (int i = 0; i < n; ++i) {
        cout << "\n-- Course " << (i + 1) << " --\n";
        s.courses.push_back(makeCourseInteractive());
    }
    rec.semesters.push_back(move(s));
    cout << "\n✅ Semester added.\n";
}

static void showReport(const StudentRecord &rec) {
    cout << "\n==================== REPORT ====================\n";
    if (rec.semesters.empty()) {
        cout << "No semesters added yet.\n";
        cout << "================================================\n";
        return;
    }
    cout.setf(std::ios::fixed); cout << setprecision(2);

    for (size_t i = 0; i < rec.semesters.size(); ++i) {
        const auto &s = rec.semesters[i];
        cout << "\n[" << (i + 1) << "] " << s.title << "\n";
        cout << left << setw(32) << "Course"
             << setw(10) << "Credits"
             << setw(8)  << "Grade"
             << setw(12) << "Points" << "\n";
        cout << string(62, '-') << "\n";
        for (const auto &c : s.courses) {
            cout << left << setw(32) << c.name.substr(0, 31)
                 << setw(10) << c.credits
                 << setw(8)  << c.gradeText
                 << setw(12) << c.gradePoint << "\n";
        }
        cout << string(62, '-') << "\n";
        cout << "Total Credits: " << s.totalCredits()
             << " | Semester GPA: " << s.gpa() << "\n";
    }
    cout << "\n-----------------------------------------------\n";
    cout << "Overall CGPA: " << rec.cgpa() << "\n";
    cout << "================================================\n";
}

static void searchCourses(const StudentRecord &rec) {
    if (rec.semesters.empty()) { cout << "Add data first.\n"; return; }
    string q = inputLineTrimmed("Enter part of course name to search: ");
    if (q.empty()) { cout << "Nothing to search.\n"; return; }
    cout << "\nMatches:\n";
    cout.setf(std::ios::fixed); cout << setprecision(2);
    bool any = false;
    for (size_t si = 0; si < rec.semesters.size(); ++si) {
        const auto &s = rec.semesters[si];
        for (const auto &c : s.courses) {
            if (icontains(c.name, q)) {
                any = true;
                cout << "• [" << (si + 1) << "] " << s.title
                     << "  |  " << c.name
                     << "  |  Credits: " << c.credits
                     << "  |  Grade: " << c.gradeText
                     << "  |  Points: " << c.gradePoint << "\n";
            }
        }
    }
    if (!any) cout << "No courses matched.\n";
}

static void sortSemester(StudentRecord &rec) {
    if (rec.semesters.empty()) { cout << "Add data first.\n"; return; }
    int sidx = inputIntInRange(
        "Choose semester index to sort (1.."
        + to_string((int)rec.semesters.size()) + "): ",
        1, (int)rec.semesters.size()) - 1;

    cout << "Sort by: 1-Name  2-Credits  3-Grade Points\n";
    int by = inputIntInRange("Choose (1-3): ", 1, 3);
    cout << "Order: 1-Ascending  2-Descending\n";
    int ord = inputIntInRange("Choose (1-2): ", 1, 2);
    bool asc = (ord == 1);

    auto &vec = rec.semesters[sidx].courses;
    auto cmpName = [&](const Course &a, const Course &b){
        return asc ? upper(a.name) < upper(b.name) : upper(a.name) > upper(b.name);
    };
    auto cmpCredits = [&](const Course &a, const Course &b){
        return asc ? a.credits < b.credits : a.credits > b.credits;
    };
    auto cmpPoints = [&](const Course &a, const Course &b){
        return asc ? a.gradePoint < b.gradePoint : a.gradePoint > b.gradePoint;
    };

    switch (by) {
        case 1: sort(vec.begin(), vec.end(), cmpName); break;
        case 2: sort(vec.begin(), vec.end(), cmpCredits); break;
        case 3: sort(vec.begin(), vec.end(), cmpPoints); break;
    }
    cout << "✅ Sorted.\n";
}

static void saveToFile(const StudentRecord &rec) {
    if (rec.semesters.empty()) { cout << "Nothing to save.\n"; return; }
    string fname = inputLineTrimmed("Save as filename (e.g., results.txt): ");
    ofstream out(fname);
    if (!out) { cout << "Could not open file for writing.\n"; return; }

    out << "CGPA_CALC_V1\n";
    out << rec.semesters.size() << "\n";
    for (const auto &s : rec.semesters) {
        out << s.title << "\n";
        out << s.courses.size() << "\n";
        for (const auto &c : s.courses) {
            // Store raw grade text; recompute gradePoint on load to keep mapping centralized
            out << c.name << "\n";
            out << setprecision(12) << c.credits << "\n";
            out << c.gradeText << "\n";
        }
    }
    cout << "✅ Saved to \"" << fname << "\".\n";
}

static void loadFromFile(StudentRecord &rec) {
    string fname = inputLineTrimmed("Open filename to load: ");
    ifstream in(fname);
    if (!in) { cout << "Could not open file.\n"; return; }

    string header; getline(in, header);
    if (header != "CGPA_CALC_V1") {
        cout << "Unrecognized file format.\n"; return;
    }

    string line;
    if (!getline(in, line)) { cout << "Corrupt file.\n"; return; }
    int semCount = 0;
    try { semCount = stoi(line); } catch (...) { cout << "Corrupt file.\n"; return; }

    vector<Semester> loaded;
    for (int i = 0; i < semCount; ++i) {
        Semester s;
        if (!getline(in, s.title)) { cout << "Corrupt file.\n"; return; }

        if (!getline(in, line)) { cout << "Corrupt file.\n"; return; }
        int ccount = 0;
        try { ccount = stoi(line); } catch (...) { cout << "Corrupt file.\n"; return; }

        for (int j = 0; j < ccount; ++j) {
            Course c;
            if (!getline(in, c.name)) { cout << "Corrupt file.\n"; return; }
            if (!getline(in, line))   { cout << "Corrupt file.\n"; return; }
            try { c.credits = stod(line); } catch (...) { cout << "Corrupt file.\n"; return; }
            if (!getline(in, c.gradeText)) { cout << "Corrupt file.\n"; return; }
            c.gradeText = upper(c.gradeText);
            c.gradePoint = Semester::gradeToPoint(c.gradeText);
            if (c.gradePoint < 0) {
                cout << "Warning: Unknown grade \"" << c.gradeText
                     << "\" in file. Setting points=0.\n";
                c.gradePoint = 0.0;
            }
            s.courses.push_back(move(c));
        }
        loaded.push_back(move(s));
    }

    rec.semesters = move(loaded);
    cout << "✅ Loaded " << rec.semesters.size() << " semester(s) from \"" << fname << "\".\n";
}

static void editCourseGrade(StudentRecord &rec) {
    if (rec.semesters.empty()) { cout << "Add data first.\n"; return; }
    int sidx = inputIntInRange(
        "Choose semester index (1.." + to_string((int)rec.semesters.size()) + "): ",
        1, (int)rec.semesters.size()) - 1;
    auto &s = rec.semesters[sidx];
    if (s.courses.empty()) { cout << "No courses in that semester.\n"; return; }

    cout << "Courses:\n";
    for (size_t i = 0; i < s.courses.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << s.courses[i].name
             << "  (Grade " << s.courses[i].gradeText
             << ", Credits " << s.courses[i].credits << ")\n";
    }
    int cidx = inputIntInRange(
        "Choose course index to edit grade (1.." + to_string((int)s.courses.size()) + "): ",
        1, (int)s.courses.size()) - 1;

    while (true) {
        string g = upper(inputLineTrimmed("New grade: "));
        double gp = Semester::gradeToPoint(g);
        if (gp >= 0) {
            s.courses[cidx].gradeText = g;
            s.courses[cidx].gradePoint = gp;
            cout << "✅ Updated.\n";
            break;
        }
        cout << "  ! Invalid grade. Try again.\n";
    }
}

// ---------- Menu ----------
static void printMenu() {
    cout << "\n========== CGPA CALCULATOR ==========\n"
         << "1) Add Semester\n"
         << "2) View Report (Sem GPAs & CGPA)\n"
         << "3) Search Course by Name\n"
         << "4) Sort Courses in a Semester\n"
         << "5) Edit a Course Grade\n"
         << "6) Save to File\n"
         << "7) Load from File\n"
         << "8) Clear All Data\n"
         << "0) Exit\n"
         << "=====================================\n";
}

int main() {
    // REMOVE these two lines for now:
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    StudentRecord rec;

    while (true) {
        printMenu();
        int choice = inputIntInRange("Choose: ", 0, 8);
        switch (choice) {
            case 1: addSemester(rec); break;
            case 2: showReport(rec); break;
            case 3: searchCourses(rec); break;
            case 4: sortSemester(rec); break;
            case 5: editCourseGrade(rec); break;
            case 6: saveToFile(rec); break;
            case 7: loadFromFile(rec); break;
            case 8: rec.clear(); cout << "✅ Cleared.\n"; break;
            case 0: cout << "Goodbye!\n"; return 0;
        }
    }
}