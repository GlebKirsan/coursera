#include "database.h"

#include <algorithm>
using namespace std;

void Database::Add(const Date &date, const std::string &event) {
    if (db.count(date) == 0) {
        db[date].push_back(event);
        db_sorted[date].insert(event);
    }
    else {
        if (db_sorted[date].find(event) != end(db_sorted[date]))
            return;
        db[date].push_back(event);
        db_sorted[date].insert(event);
    }
}

void Database::Print(std::ostream &os) const {
    for(const auto& event : db)
        os << event;
}

void Database::CheckFirst(const Date& last) const {
    if (db.empty())
        return;

    Date firstInDB = begin(db)->first;
    if (last < firstInDB)
        throw invalid_argument("");
}

pair<Date, vector<string>> Database::GetLowerBoundPair(const Date& last) const {
    auto it = db.lower_bound(last);

    if (it->first == last)
        return *it;
    if (it != begin(db))
        return *prev(it);
    else
        throw invalid_argument("");
}

pair<Date, string> Database::Last(const Date &last) const {
    CheckFirst(last);

    auto lw_bound = GetLowerBoundPair(last);
    size_t n = lw_bound.second.size();
    return {lw_bound.first, lw_bound.second[n-1]};
}

