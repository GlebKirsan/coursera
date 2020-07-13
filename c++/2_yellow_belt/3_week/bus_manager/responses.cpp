#include "responses.h"
#include <iostream>
using namespace std;

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