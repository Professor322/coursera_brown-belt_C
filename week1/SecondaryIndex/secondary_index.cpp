#include "test_runner.h"

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

struct RecordWithSecondaryIndecies {
	Record rec;
	multimap<string, Record*>:: iterator it_user;
	multimap<int, Record*>:: iterator it_karma;
	multimap<int, Record*>:: iterator it_timestamp;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record) {
  	if (db_main.count(record.id)) {
  		return false;
  	}
  	db_main.insert({record.id, {record}});
  	auto& elem = db_main[record.id];

  	elem.it_user = db_user.insert({record.user, &elem.rec});
  	elem.it_karma = db_karma.insert({record.karma, &elem.rec});
  	elem.it_timestamp = db_timestamp.insert({record.timestamp, &elem.rec});
  	return true;
  }
  const Record* GetById(const string& id) const {
  	if (!db_main.count(id)) {
  		return nullptr;
  	}
  	return &db_main.at(id).rec;
  }
  bool Erase(const string& id) {
  	if (!db_main.count(id)) {
  		return false;
  	}
  	auto& elem = db_main.at(id);
  	db_timestamp.erase(elem.it_timestamp);
  	db_karma.erase(elem.it_karma);
  	db_user.erase(elem.it_user);
	db_main.erase(id);
	return true;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
  	RangeHelper(db_timestamp, low, high, callback);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
  	RangeHelper(db_karma, low, high, callback);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
	  auto[begin, end] = db_user.equal_range(user);
	  for (; begin != end; ++begin) {
		  if (!callback(*begin->second)) {
			  return;
		  }
	  }
  }

private:

	template <typename Callback>
	void RangeHelper(const multimap<int, Record*>& mmap, int low, int high, Callback callback) const {
		auto begin = mmap.lower_bound(low);
		auto end = mmap.upper_bound(high);

		for (; begin != end; ++begin) {
			if (!callback(*begin->second)) {
				return ;
			}
		}

	}

	unordered_map<string, RecordWithSecondaryIndecies> db_main;
	multimap<string, Record*> db_user;
	multimap<int, Record*> db_timestamp;
	multimap<int, Record*> db_karma;
};

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
