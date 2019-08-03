#ifndef W6_DATE_H
#define W6_DATE_H

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

class Date {
public:
    Date();
    Date(const int year, const int month, const int day);

    bool operator==(const Date& lhs) const;
    bool operator!=(const Date& lhs) const;
    bool operator<(const Date& lhs) const;
    bool operator>(const Date& lhs) const;
    bool operator>=(const Date& lhs) const;
    bool operator<=(const Date& lhs) const;

    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
private:
    int year;
    int month;
    int day;
};

Date ParseDate(std::istream& is);

inline std::ostream& operator<<(std::ostream& os, const Date &a) {
    os << std::setw(4) << std::setfill('0') << a.GetYear() << '-' <<
    std::setw(2) << std::setfill('0') <<a.GetMonth() << '-'  <<
    std::setw(2) << std::setfill('0') << a.GetDay();
    return os;
}

inline std::ostream& operator<<(
        std::ostream& os,
        const std::pair<Date, std::string> events){
    os << events.first << ' ' << events.second;
    return os;
}

inline std::ostream& operator<<(
        std::ostream& os,
        const std::pair<Date, std::vector<std::string>> events){
    for(const std::string &event : events.second)
        os << events.first << ' ' << event << std::endl;
    return os;
}

#endif //W6_DATE_H
