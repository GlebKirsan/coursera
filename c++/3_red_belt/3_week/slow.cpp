#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

#define AS_KV(x) #x << " = " << x


class ReadingManager {
public:
  ReadingManager()
      : users(MAX_USERS, 0),
        pages_raiting(MAX_PAGES, 0){}

  void Read(int user_id, int page_count) {
      int& page = users[user_id];
      if (page != 0){
          --pages_raiting[page];
      } else {
          ++real_users;
      }
      users[user_id] = page_count;
      ++pages_raiting[page_count];
  } 

  double Cheer(int user_id) const {
      const int& page = users[user_id];
      if (page == 0){
          return 0;
      }
      if (real_users == 1){
          return 1;
      }
      int sum_of_viewers = 0;
      for (int i = 0; 
           i < MAX_USERS && i < page; 
           ++i){
          sum_of_viewers += pages_raiting[i];
      }
      return sum_of_viewers * 1.0 / (real_users - 1);
  }
  
private:
  static const int MAX_USERS = 1'000'000;
  static const int MAX_PAGES = 1'000;
  int real_users = 0;


  vector<int> users;
  vector<int> pages_raiting;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
        cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }
  return 0;
}
