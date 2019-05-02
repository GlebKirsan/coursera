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

//Compile error: b"/usr/bin/ld: /tmp/full-5f02a4.o: in function `main':\nfull.cpp:(.text+0x263): undefined reference to
//`operator<<(std::ostream&, AllBusesResponse const&)'\n/usr/bin/ld: /tmp/main-af524c.o: in function
// `main_9064003()':\nmain.cpp:(.text+0x263): undefined reference to `operator<<(std::ostream&, AllBusesResponse const&)
// '\nclang: fatal error: linker command failed with exit code 1 (use -v to see invocation)\n"