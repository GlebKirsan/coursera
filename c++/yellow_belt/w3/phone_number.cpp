#include "phone_number.h"
#include <sstream>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

PhoneNumber::PhoneNumber(const string &international_number) {
    stringstream s(international_number);

    char sign = s.get();
    getline(s, country_code_, '-');
    getline(s, city_code_, '-');
    getline(s, local_number_);

    if (sign != '+' ||
    country_code_.empty() ||
    city_code_.empty() ||
    local_number_.empty()){
        throw invalid_argument("");
    }
//    bool ok = true;
//    int co_code;
//    int ci_code;
//    string lo_code;
//    ok = ok && (s.peek() == '+');
//    ok = ok && (s >> co_code);
//
//    ok = ok && (s.peek() == '-');
//    s.ignore(1);
//    ok = ok && (s >> ci_code);
//
//    ok = ok && (s.peek() == '-');
//    s.ignore(1);
//    ok = ok && (s >> lo_code);
//
//    if (!ok){
//        throw invalid_argument("");
//    }
//    country_code_= to_string(co_code);
//    city_code_ = to_string(ci_code);
//    local_number_ = lo_code;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetInternationalNumber() const {
    return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}