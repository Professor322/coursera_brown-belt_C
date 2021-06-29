#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <list>

using namespace std;


template <typename T>
class PriorityCollection {
public:
	using Id = int;/* тип, используемый для идентификаторов */;

	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object) {
		const Id new_id = objects.size();
		const int starting_priority = 0;

		objects.push_back({starting_priority, move(object)});
		sorted_objects.insert({starting_priority, new_id});
		return new_id;
	}

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
			 IdOutputIt ids_begin) {
		for (auto it = range_begin; it != range_end; ) {
			*ids_begin = Add(move(*it));
			ids_begin++;
			it++;
		}

	}

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const {
		return id >= 0 && id < objects.size() &&
				objects[id].priority != INVALID_PRIORITY;
	}

	// Получить объект по идентификатору
	const T& Get(Id id) const {
		return objects[id].element;
	}

	// Увеличить приоритет объекта на 1
	void Promote(Id id) {
		auto& elem = objects[id];
		int old_priority = elem.priority;
		int new_priority = ++elem.priority;

		sorted_objects.erase({old_priority, id});
		sorted_objects.insert({new_priority, id});
	}

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, int> GetMax() const {
		int id = prev(sorted_objects.end())->second;

		return {objects[id].element, objects[id].priority};
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax() {
		auto it = prev(sorted_objects.end());
		int priority = it->first;
		T object = move(objects[it->second].element);
		objects[it->second].priority = INVALID_PRIORITY;
		sorted_objects.erase(it);

		return {move(object), priority};
	}

private:
	// Приватные поля и методы
	struct Object {
		int priority;
		T element;
	};
	vector<Object> objects;
	set<pair<int, Id>> sorted_objects;
	static const int INVALID_PRIORITY = -1;

};

class StringNonCopyable : public string {
public:
	using string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}
/*
int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	return 0;
}*/
