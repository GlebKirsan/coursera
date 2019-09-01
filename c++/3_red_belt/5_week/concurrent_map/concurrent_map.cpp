#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <random>
#include <future>

using namespace std;

template<typename K, typename V>
class ConcurrentMap {
public:
    static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

    struct Access {
        V &ref_to_value;
        lock_guard<mutex> guard;
    };

    struct VectorAccess {
        map<K, V> &ref_to_value;
        lock_guard<mutex> guard;
    };

    explicit ConcurrentMap(size_t bucket_count) :
            container(bucket_count),
            parts(bucket_count * value_per_page, 0) {
        int page = 1;
        for (int i = value_per_page; i < parts.size();) {
            parts[i] = page;
            if (++i % value_per_page == 0) {
                page++;
            }
        }
    }

    Access operator[](const K &key) {
        VectorAccess to_vector{container[parts[key]], lock_guard(vector_access)};
        return {to_vector.ref_to_value[key],
                lock_guard(value_access)};
    }

    map<K, V> BuildOrdinaryMap() {
        map<K, V> ordinary_map;
        for (auto &m : container) {
            VectorAccess va{m, lock_guard(vector_access)};
            for (auto&[key, value] : m) {
                Access lul{value, lock_guard(value_access)};
                ordinary_map[key] = lul.ref_to_value;
            }
        }
        return ordinary_map;
    }

private:
    mutex value_access;
    mutex vector_access;
    static const int value_per_page = 10000;

    vector<int> parts;
    vector<map<K, V>> container;
};

void RunConcurrentUpdates(
        ConcurrentMap<int, int> &cm, size_t thread_count, int key_count
) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), default_random_engine(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                cm[key].ref_to_value++;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    const size_t thread_count = 3;
    const size_t key_count = 50000;

    ConcurrentMap<int, int> cm(thread_count);
    RunConcurrentUpdates(cm, thread_count, key_count);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto&[k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value += 'a';
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2}) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string &s) {
            return s.empty() || s == "a" || s == "aa";
        }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}
