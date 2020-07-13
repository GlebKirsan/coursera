#pragma once

#include <map>
#include <string>
#include <vector>

using dict = std::map<std::string, std::vector<std::string>>;


struct BusesForStopResponse {
    dict stops_to_buses;
    std::string stop;
};

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r);

struct StopsForBusResponse {
    dict buses_to_stops;
    dict stops_to_buses;
    std::string bus;
};

std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r);

struct AllBusesResponse {
    dict buses_to_stops;
};

std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r);