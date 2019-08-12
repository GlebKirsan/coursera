#include "bus_manager.h"
#include "responses.h"

using namespace std;

void BusManager::AddBus(const string &bus, const vector<string> &stops) {
    stb.buses_to_stops[bus].resize(stops.size());
    for(size_t s = 0; s < stops.size(); ++s){
        stb.buses_to_stops[bus][s] = stops[s];
        bts.stops_to_buses[stops[s]].push_back(bus);
    }
    stb.stops_to_buses = bts.stops_to_buses;
    abr.buses_to_stops = stb.buses_to_stops;
}

BusesForStopResponse BusManager::GetBusesForStop(const string &stop) const{
    BusesForStopResponse ans = bts;
    ans.stop = stop;
    return ans;
}

StopsForBusResponse BusManager::GetStopsForBus(const string &bus) const{
    StopsForBusResponse ans = stb;
    ans.bus = bus;
    return ans;
}

AllBusesResponse BusManager::GetAllBuses() const {
    return abr;
}