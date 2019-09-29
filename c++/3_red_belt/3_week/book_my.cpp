#include <iostream>
#include <cstdint>
#include <deque>
#include <map>
#include <set>

using namespace std;

struct Customer {
    int64_t time;
    string hotel_name;
    int client_id;
    int room_count;
};

istream& operator>>(istream& in, Customer& cust){
    in >> cust.time >> cust.hotel_name >> cust.client_id 
        >> cust.room_count;
    return in;
}

class Book {
public:
    Book(){}

    void AddClient(){
        Customer c;
        cin >> c;
        Clear(c.hotel_name);
        hotel_to_customers[c.hotel_name].push_back(c);
        hotel_to_rooms[c.hotel_name] += c.room_count;
        unique_customers[c.hotel_name].insert(c.client_id);
        num_of_visits[c.hotel_name][c.client_id]++;
        current_time = c.time;
    }

    void GetCustomersInHotel(const string& hotel_name) {
        if (hotel_to_customers.count(hotel_name) == 0){
            cout << 0 << endl;
            return;
        }
        Clear(hotel_name);
        cout << unique_customers[hotel_name].size() << endl;
    }

    void GetReservedRoomsInHotel(const string& hotel_name) {
        if (hotel_to_customers.count(hotel_name) == 0){
            cout << 0 << endl;
            return;
        }
        Clear(hotel_name);
        cout << hotel_to_rooms[hotel_name] << endl;
    }

private:
    static const int threshold = 86'400;
    int64_t current_time;

    void Clear(const string& hotel_name) {
        auto it = hotel_to_customers.find(hotel_name);
        auto rooms = hotel_to_rooms.find(hotel_name);
        auto unique_customers_ = unique_customers.find(hotel_name);
        auto nov = num_of_visits.find(hotel_name);

        if (it == end(hotel_to_customers)){
            return;
        }

        while (!it->second.empty() &&
                it->second.front().time + threshold <= current_time) {
            rooms->second -= it->second.front().room_count;
            auto tmp = nov->second.find(it->second.front().client_id);
            --tmp->second;
            if (tmp->second == 0){
                unique_customers_->second.erase(it->second.front().client_id);
            }
            it->second.pop_front();
        }
    }

    map<string, int> hotel_to_rooms;
    map<string, deque<Customer>> hotel_to_customers;
    map<string, set<int>> unique_customers;
    map<string, map<int, int>> num_of_visits;
};

int main(){
    std::ios_base::sync_with_stdio(0);
    cin.tie(nullptr);
    Book book;

    int q;
    cin >> q;
    
    for(int query_count = 0; query_count < q; ++query_count){
        string query_type;
        cin >> query_type;
        string hotel_name;

        if (query_type == "BOOK"){
            book.AddClient();
        } else if (query_type == "CLIENTS") {
            cin >> hotel_name;
            book.GetCustomersInHotel(hotel_name);
        } else if (query_type == "ROOMS") {
            cin >> hotel_name;
            book.GetReservedRoomsInHotel(hotel_name);
        }
    }
}
