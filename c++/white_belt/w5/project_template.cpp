#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include <set>
#include <map>
using namespace std;
const regex reg = regex(R"(-?\d+--?\d+--?\d+)");

class Date {
public:
    Date() = default;

    Date(int new_year, int new_month, int new_day) {
        year = new_year;
        if (1 > new_month || new_month > 12) {
            throw invalid_argument("Month value is invalid: " + to_string(new_month));
        }
        month = new_month;
        if (new_day < 1 || new_day > 31) {
            throw invalid_argument("Day value is invalid: " + to_string(new_day));
        }
        day = new_day;
    }

    bool operator<(const Date &rhs) const {
        if (year != rhs.year) {
            return year < rhs.year;
        }
        if (month != rhs.month) {
            return month < rhs.month;
        }
        return day < rhs.day;
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

private:
    int year;
    int month;
    int day;
};

ostream &operator<<(ostream &ostr, const Date &date) {
    ostr << setfill('0');
    return ostr << setw(4) << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
}

void check(string &input) {
    if (!regex_match(input, reg)) {
        throw invalid_argument("Wrong date format: " + input);
    }
}

istream &operator>>(istream &istr, Date &date) {
    int year;
    int month;
    int day;
    string input;
    istr >> input;
    check(input);
    stringstream inp(input);
    inp >> year;
    inp.ignore(1);
    inp >> month;
    inp.ignore(1);
    inp >> day;
    date = Date(year, month, day);
    return istr;
}

Date ParseDate(stringstream &s) {
    Date date;
    s >> date;
    return date;
}

class Database {
public:
    void AddEvent(const Date &date, const string &event) {
        if (!(db.count(date) > 0 && db.at(date).count(event) > 0)) {
            db[date].insert(event);
        }
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (db.count(date) && db[date].count(event)) {
            db[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date &date) {
        if (db.count(date)) {
            int n = db[date].size();
            db.erase(date);
            return n;
        }
        return 0;
    }

    void Find(const Date &date) const {
        if (db.count(date)) {
            for (const auto &event: db.at(date)) {
                cout << event << "\n";
            }
        }
    }

    void Print() const {
        for (const auto &k: db) {
            for (auto const &event: k.second) {
                cout << k.first << " " << event << "\n";
            }
        }
    }

private:
    map<Date, set<string>> db;
};

int main() {
    Database db;
    string command, operation;
    while (getline(cin, command)) {
        if (command.empty()) {
            continue;
        }
        stringstream stream(command);
        stream >> operation;
        if (operation == "Print") {
            db.Print();
        } else if (operation != "Del" && operation != "Add" && operation != "Find") {
            cout << "Unknown command: " << operation << "\n";
            break;
        } else {
            Date date;
            try {
                date = ParseDate(stream);
            } catch (invalid_argument &e) {
                cout << e.what() << "\n";
                break;
            }
            if (operation == "Find") {
                db.Find(date);
            } else {
                string event;
                stream >> event;
                if (operation == "Add") {
                    db.AddEvent(date, event);
                } else {
                    if (event.empty()) {
                        cout << "Deleted " << db.DeleteDate(date) << " events\n";
                    } else {
                        cout << (db.DeleteEvent(date, event) ? "Deleted successfully" : "Event not found") << "\n";
                    }
                }
            }
        }
    }
    return 0;
}