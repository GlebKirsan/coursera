#include <test_runner.h>

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
public:
    bool Put(const Record& record);

    const Record* GetById(const string& id) const;

    bool Erase(const string& id);

    template<typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template<typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template<typename Callback>
    void AllByUser(const string& user, Callback callback) const;

private:

    unordered_map<string, Record> id_to_record;
    multimap<string, Record&> user_to_records;
    multimap<int, Record&> karma_to_records;
    multimap<int, Record&> timestamp_to_records;
};

bool Database::Put(const Record& record) {
    if (id_to_record.count(record.id) != 0) {
        return false;
    }

    id_to_record[record.id] = record;
    user_to_records.insert({record.user, id_to_record[record.id]});
    karma_to_records.insert({record.karma, id_to_record[record.id]});
    timestamp_to_records.insert({record.timestamp, id_to_record[record.id]});
    return true;
}

const Record* Database::GetById(const string& id) const {
    if (id_to_record.count(id) > 0) {
        return &id_to_record.find(id)->second;
    } else {
        return nullptr;
    }
}

bool Database::Erase(const string& id) {
    if (id_to_record.count(id) == 0) {
        return false;
    }
    auto& record = id_to_record[id];
    auto user_range = user_to_records.equal_range(record.user);
    for (auto it = user_range.first; it != user_range.second; ++it) {
        if (it->second.id == id) {
            user_to_records.erase(it);
            break;
        }
    }
    karma_to_records.erase(record.karma);
    timestamp_to_records.erase(record.timestamp);

    id_to_record.erase(id);

    return true;
}

template<typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
    auto l_bound = timestamp_to_records.lower_bound(low);
    auto u_bound = timestamp_to_records.upper_bound(high);

    for (auto it = l_bound; it != u_bound; ++it) {
        if (!callback(it->second)) {
            break;
        }
    }
}

template<typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
    auto l_bound = karma_to_records.lower_bound(low);
    auto u_bound = karma_to_records.upper_bound(high);

    for (auto it = l_bound; it != u_bound; ++it) {
        if (!callback(it->second)) {
            break;
        }
    }
}

template<typename Callback>
void Database::AllByUser(const string& user, Callback callback) const {
    auto range = user_to_records.equal_range(user);
    for (auto it = range.first; it != range.second; ++it) {
        if (!callback(it->second)) {
            break;
        }
    }
}

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
