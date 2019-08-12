#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

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
    cin >> type;
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

struct BusesForStopResponse {
    dict stops_to_buses;
    string stop;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r) {
    if (r.stops_to_buses.count(r.stop) == 0) {
        os << "No stop";
    } else {
        for (const string &bus: r.stops_to_buses.at(r.stop)) {
            os << bus << " ";
        }
    }
    return os;
}

struct StopsForBusResponse {
    dict buses_to_stops;
    dict stops_to_buses;
    string bus;
};

ostream &operator<<(ostream &os, const StopsForBusResponse &r) {
    if (r.buses_to_stops.count(r.bus) == 0) {
        os << "No bus";
    } else {
        for (size_t s = 0; s < r.buses_to_stops.at(r.bus).size(); ++s) {
            const string &stop = r.buses_to_stops.at(r.bus)[s];
            os << "Stop " << stop << ": ";
            if (r.stops_to_buses.at(stop).size() == 1) {
                os << "no interchange";
            } else {
                for (const string &other_bus : r.stops_to_buses.at(stop)){
                    if (r.bus != other_bus){
                        os << other_bus << " ";
                    }
                }
            }
            if (s + 1 != r.buses_to_stops.at(r.bus).size()) {
                os << endl;
            }
        }
    }
    return os;
}

struct AllBusesResponse {
    dict buses_to_stops;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r) {
    if (r.buses_to_stops.empty()){
        os << "No buses";
    } else {
        for (auto &bus_item : r.buses_to_stops){
            os << "Bus " << bus_item.first << ": ";
            for (const string &stop : bus_item.second) {
                os << stop << " ";
            }
            os << endl;
        }

    }
    return os;
}

class BusManager {
public:
    void AddBus(const string &bus, const vector<string> &stops) {
        stb.buses_to_stops[bus].resize(stops.size());
        for(size_t s = 0; s < stops.size(); ++s){
            stb.buses_to_stops[bus][s] = stops[s];
            bts.stops_to_buses[stops[s]].push_back(bus);
        }
        stb.stops_to_buses = bts.stops_to_buses;
        abr.buses_to_stops = stb.buses_to_stops;
    }

    BusesForStopResponse GetBusesForStop(const string &stop) const{
        BusesForStopResponse ans = bts;
        ans.stop = stop;
        return ans;
    }

    StopsForBusResponse GetStopsForBus(const string &bus) const{
        StopsForBusResponse ans = stb;
        ans.bus = bus;
        return ans;
    }

    AllBusesResponse GetAllBuses() const {
        return abr;
    }

private:
    BusesForStopResponse bts;
    StopsForBusResponse stb;
    AllBusesResponse abr;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}
