#ifndef W6_DATABASE_H
#define W6_DATABASE_H
#include "date.h"

//#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <sstream>

class Database {
public:
    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& os) const;
    std::pair<Date, std::string> Last(const Date& last) const;

    void CheckFirst(const Date& last) const;
    std::pair<Date, std::vector<std::string>> GetLowerBoundPair(const Date& last) const;


    template <typename T>
    int RemoveIf(const T& condition) {
        int counter = 0;
        std::vector<Date> dates_to_erase;
        for(auto& evs : db){
            Date date(evs.first);
            auto it = std::stable_partition(begin(evs.second), end(evs.second),
                    [date, condition](const std::string& event){
                return !condition(date, event);
            });
            counter += std::distance(it, end(evs.second));
            for(auto erasion_it = it; erasion_it != end(evs.second); ++erasion_it)
                db_sorted[date].erase(*erasion_it);

            if (it == begin(evs.second))
                dates_to_erase.push_back(evs.first);
            else
                evs.second = {begin(evs.second), it};
        }
        for(const auto& date : dates_to_erase) {
            db.erase(date);
            db_sorted.erase(date);
        }
        return counter;
    }

    template <typename T>
    std::vector<std::pair<Date, std::string>> FindIf(const T& condition) const{
        std::vector<std::pair<Date, std::string>> finded;
        for(const auto& evs : db){
            Date date(evs.first);
            std::vector<std::string> events;
            std::copy_if(begin(evs.second), end(evs.second), std::back_inserter(events),
                    [date, condition](const std::string& event){
                return condition(date, event);
            });
            if (!events.empty())
                for(const auto& ev : events)
                    finded.push_back({evs.first, ev});
        }
        return finded;
    }

private:
    std::map<Date, std::vector<std::string>> db;
    std::map<Date, std::set<std::string>> db_sorted;
};

#endif //W6_DATABASE_H