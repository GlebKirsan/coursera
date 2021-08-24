#include <utility>
#include <map>

#include "Common.h"

using namespace std;

struct RankedBookBtr {
        ICache::BookPtr book;
        size_t rank;
};

class LruCache : public ICache {
private:
        Settings settings_;
        shared_ptr<IBooksUnpacker> books_unpacker;
        map<string, RankedBookBtr> cached_library;
        size_t current_max_rank = 0;

public:
        LruCache(
                shared_ptr<IBooksUnpacker> books_unpacker,
                const Settings &settings
        ) : settings_(settings), books_unpacker(std::move(books_unpacker)) {
        }

        BookPtr GetBook(const string &book_name) override {
                // реализуйте метод
                if (cached_library.count(book_name) != 0) {
                        cached_library[book_name].rank = ++current_max_rank;
                        return cached_library[book_name].book;
                }
                BookPtr unpackedBook = books_unpacker->UnpackBook(book_name);
                return unpackedBook;
        }
};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings &settings
) {
        return make_unique<LruCache>(books_unpacker, settings);
}
