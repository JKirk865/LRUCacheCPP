#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "LRUCache.h"
#include "LRUCacheUnitTests.h"

using namespace std;

namespace LRUCacheLib {

	template<class K, class V>
	LRUCacheLib::LRUCache<K, V>::LRUCache(size_t Capacity) {
		capacity = Capacity;
	}

	template <class K, class V>
	LRUCache<K,V>::~LRUCache() {
		const lock_guard<mutex> lock(classpadlock);
		items.clear();
		cache.clear();
	}

	template <class K, class V>
	size_t LRUCache<K,V>::Capacity()
	{
		const lock_guard<mutex> lock(classpadlock);
		return capacity;
	}

	template <class K, class V>
	void LRUCache<K,V>::Clear()
	{
		const lock_guard<mutex> lock(classpadlock);
		items.clear();
		cache.clear();
	}

	template <class K, class V>
	size_t LRUCache<K,V>::Count()
	{
		const lock_guard<mutex> lock(classpadlock);
		return cache.size();
	}

	template <class K, class V>
	optional<V> LRUCache<K,V>::Get(K key)
	{
		const lock_guard<mutex> lock(classpadlock);

		auto it = items.find(key);
		if (it != items.end())
		{
			cache.remove(key);
			cache.push_back(key);
			return it->second;
		}
		return std::nullopt;
	}

	template <class K, class V>
	bool LRUCache<K,V>::Remove(K key)
	{
		const lock_guard<mutex> lock(classpadlock);

		auto it = items.find(key);
		if (it != items.end())
		{
			cache.remove(key);
			items.erase(key);
			return true;
		}

		return false;
	}

	template <class K, class V>
	void LRUCache<K,V>::Put(K key, V value)
	{
		const lock_guard<mutex> lock(classpadlock);

		auto it = items.find(key);
		if (it != items.end())
		{
			cache.remove(key);
			cache.push_back(key);
			it->second = value;
			return;
		}

		//Add a new node
		cache.push_back(key);
		items.insert(pair<K,V>(key, value));

		// Check size of cache
		if (cache.size() > capacity)
		{
			int tmp_key = cache.front();
			items.erase(tmp_key);
			cache.pop_front();
		}
	}

	template <class K, class V>
	list<pair<K, V>> LRUCache<K,V>::ToList()
	{
		const lock_guard<mutex> lock(classpadlock);

		list<pair<K, V>>  list(0);
		for(auto x : cache)
		{
			auto it = items.find(x);
			list.push_back(pair<int, string>(it->first, it->second));
		}
		return list;
	}

int main_old()
{
	cout << "LRUCache Tests" << endl;

	LRUCacheLib::LRUCache<int, string> c(1'000);
	c.Put(1,"red");
	c.Put(2,"Blue");
	cout << "Cache has " << c.Count() << " entries." << endl;
	LRUCacheUnitTests::DumpList(c.ToList(), "After creation");


	auto v = c.Get(1);
    cout << "Got Key 1, value=" << v.value_or("Value Not FOund") << endl;
    //LRUCacheUnitTests::DumpList<int, string>(c.ToList(), "After Get");

	c.Remove((int)1);
	cout << "Remove Key 1" << endl;
	cout << "Cache has " << c.Count() << " entries." << endl;
	LRUCacheUnitTests::DumpList(c.ToList(), "After removal");

	c.Clear();
	LRUCacheUnitTests::PutTest(c);
	//LRUCacheUnitTests::DumpList(c.ToList(), "After PutTest");
	LRUCacheUnitTests::GetTest(c);
	LRUCacheUnitTests::RemoveTest(c);

	cout << "main_old LRUCache Tests Finished" << endl;
	return 0;
}
} /* namespace LRUCacheLib */

