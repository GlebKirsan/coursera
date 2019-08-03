#include "date.h"
#include <algorithm>
#include <tuple>
using namespace std;

Date::Date() {
    year = 0;
    month = 0;
    day = 0;
}

Date::Date(
        const int year,
        const int month,
        const int day):
        year(year),
        month(month),
        day(day){}

bool Date::operator==(const Date &lhs) const {
    return
        tie(year, month, day) ==
        tie(lhs.year, lhs.month, lhs.day);
}

bool Date::operator!=(const Date &lhs) const {
    return !operator==(lhs);
}

bool Date::operator<(const Date &lhs) const {
    return tie(year, month, day) < tie(lhs.year, lhs.month, lhs.day);
}

bool Date::operator<=(const Date &lhs) const {
    return operator<(lhs) || operator==(lhs);
}

bool Date::operator>(const Date &lhs) const {
    return !operator<=(lhs);
}

bool Date::operator>=(const Date &lhs) const {
    return operator>(lhs) || operator==(lhs);
}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}

Date ParseDate(istream& is){
    string date;
    is >> date;
    auto first = find(begin(date), end(date), '-');
    auto last = find(next(first), end(date), '-');
    int year = stoi(string{begin(date), first});
    int month = stoi(string{next(first), last});
    int day = stoi(string{next(last), end(date)});
    return {year, month, day};
}