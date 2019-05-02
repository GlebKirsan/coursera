#include "query.h"

#include <map>
using namespace std;

void read_stops(int stop_count, vector<string> &stops, istream &is) {
    stops.resize(stop_count * 1ul);
    for (string &stop: stops) {
        is >> stop;
    }
}

istream &operator>>(istream &is, Query &q) {
    map<string, QueryType> m = {
            {"NEW_BUS",        QueryType::NewBus},
            {"BUSES_FOR_STOP", QueryType::BusesForStop},
            {"STOPS_FOR_BUS",  QueryType::StopsForBus},
            {"ALL_BUSES",      QueryType::AllBuses}
    };
    string type;
    is >> type;
    q.type = m[type];
    switch (q.type) {
        case QueryType::NewBus:
            is >> q.bus;
            int stop_count;
            is >> stop_count;
            read_stops(stop_count, q.stops, is);
            break;
        case QueryType::BusesForStop:
            is >> q.stop;
            break;
        case QueryType::StopsForBus:
            is >> q.bus;
            break;
        case QueryType::AllBuses:
            break;
    }
    return is;
}