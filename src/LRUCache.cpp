#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

#include "LRUCache.h"

using namespace std;

namespace LRUCacheLib {

template<class K, class V>
LRUCacheLib::LRUCache<K, V>::LRUCache(size_t Capacity) {
	capacity = Capacity;
}

template<class K, class V>
LRUCache<K, V>::~LRUCache() {
	const lock_guard<mutex> lock(classpadlock);
	items.clear();
	cache.clear();
}

template<class K, class V>
size_t LRUCache<K, V>::Capacity() {
	const lock_guard<mutex> lock(classpadlock);
	return capacity;
}

template<class K, class V>
void LRUCache<K, V>::Clear() {
	const lock_guard<mutex> lock(classpadlock);
	items.clear();
	cache.clear();
}

template<class K, class V>
size_t LRUCache<K, V>::Count() {
	const lock_guard<mutex> lock(classpadlock);
	return cache.size();
}

template<class K, class V>
optional<V> LRUCache<K, V>::Get(const K & key) {
	const lock_guard<mutex> lock(classpadlock);

	auto it = items.find(key);
	if (it != items.end()) {
		cache.remove(key);
		cache.push_back(key);
		//cache.splice(cache.end(),cache, it, it); // Faster?
		return it->second;
	}
	return std::nullopt;
}

template<class K, class V>
bool LRUCache<K, V>::Remove(const K & key) {
	const lock_guard<mutex> lock(classpadlock);

	auto it = items.find(key);
	if (it != items.end()) {
		cache.remove(key);
		items.erase(key);
		return true;
	}

	return false;
}

template<class K, class V>
void LRUCache<K, V>::Put(const K & key, const V & value) {
	const lock_guard<mutex> lock(classpadlock);

	auto it = items.find(key);
	if (it != items.end()) {
		cache.remove(key);
		cache.push_back(key);
		//cache.splice(cache.end(),cache, it); // Faster?
		it->second = value;
		return;
	}

	//Add a new node
	cache.push_back(key);
	items.insert(pair<K, V>(key, value));

	// Check size of cache
	if (cache.size() > capacity) {
		int tmp_key = cache.front();
		items.erase(tmp_key);
		cache.pop_front();
	}
}

template<class K, class V>
list<pair<K, V>> LRUCache<K, V>::ToList() {
	const lock_guard<mutex> lock(classpadlock);

	list<pair<K, V>> list(0);
	for (auto x : cache) {
		auto it = items.find(x);
		list.push_back(pair<int, string>(it->first, it->second));
	}
	return list;
}

#if defined(DEBUG)

class LRUCacheUnitTests: public ::testing::Test {
public:
	LRUCacheUnitTests( ) {
		//cout << "[   LRUCacheUnitTests: NumThreads -> " << NumThreads << endl;
		//cout << "[   LRUCacheUnitTests: NumGetsPerThread -> " << NumGetsPerThread << endl;
		//cout << "[   LRUCacheUnitTests: NumPutsPerThread -> " << NumPutsPerThread << endl;
   }

   ~LRUCacheUnitTests( ) override {
	   // cleanup any pending stuff, but no exceptions allowed
   }

   void SetUp( ) override {
       // code here will execute just before the test ensues
   }

   void TearDown( ) override {
       // code here will be called just after the test completes
       // ok to through exceptions from here if need be
		threadlist.clear(); // Clear the Vector to ensure the threads are destroyed.
   }

   static void DumpList(list<pair<int, string>> l, string title)
   {
	   cout << title << endl;
	   cout << "Oldest to Newest: " << l.size() << "." << endl;
	   for(auto i : l) {
		   cout << "Key:" << i.first << " Value:" << i.second << endl;
	   }
   }

   // put in any custom data members that you need
   //LRUCache<int, string> empty_cache<int, string>(10);
	static const int NumPutsPerThread = 1'000;
	static const int NumGetsPerThread = 1'000;
	static const int NumThreads = 10;
	static const int KeyRange = 10'000;

	vector<thread> threadlist;
};


	TEST_F(LRUCacheUnitTests, SimpleTests)
	{
		LRUCacheLib::LRUCache<int, string> c(10);
		ASSERT_EQ(0, c.Count());
		c.Put(1,"red");
		ASSERT_EQ(1, c.Count());
		c.Put(2,"Blue");
		ASSERT_EQ(2, c.Count());

		// Dump the list.
		LRUCacheUnitTests::DumpList(c.ToList(), "After creation");

		// Get a Value we just put in
		auto v = c.Get(1);
		ASSERT_TRUE(v.has_value());
		ASSERT_EQ(2, c.Count());

		// Remove an item, make sure the count is 1
		c.Remove((int)1);
		auto r = c.Get(1);
		ASSERT_FALSE(r.has_value());
		ASSERT_EQ(1, c.Count());

		// Remove the last item and make sure the count is Zero
		c.Remove((int)2);
		ASSERT_EQ(0, c.Count());

	}

	TEST_F(LRUCacheUnitTests, PutTest)
	{
		std::unique_ptr<LRUCache<int, string>> cache(new LRUCache<int, string>(1'000));

		// Create a Lambda function that add unique elements to the cache
		auto PutTestFunc = [&cache](int StartInt) {
			for(int i=StartInt; i<(StartInt+NumPutsPerThread); i++) {
				cache->Put(i,to_string(i));
			}
		};

		//Create and Start the Threads
		for(int i=0; i < NumThreads; i++) {
			threadlist.push_back(thread(PutTestFunc, (i*NumPutsPerThread)+1));
		}
		//Join the threads when they are complete.
		for (std::thread & th : threadlist)
		{
			// If thread Object is Joinable then Join that thread.
			if (th.joinable())
				th.join();
		}

		ASSERT_EQ(NumThreads*NumPutsPerThread, cache->Count());
	}

	TEST_F(LRUCacheUnitTests, GetTest)
	{
	    std::unique_ptr<LRUCache<int, string>> cache(new LRUCache<int, string>(KeyRange));

	    // Add entries to the Cache
		for(int i=0; i<KeyRange; i++) {
			cache->Put(i,to_string(i));
		}

		// Create a Lambda function that attempt to find random elements from the cache
		auto GetTestFunc = [&cache]() {
			int NumFound = 0;
			for(int i=0; i<NumGetsPerThread; i++) {
				auto v = cache->Get(rand() % KeyRange);
				ASSERT_TRUE(v.has_value());
				if (v.has_value()) NumFound++;
			}
			//cout << "GetTest Thread located " << NumFound << " matches." << endl;
		};

		//Create and Start the Threads
		for(int i=0; i < NumThreads; i++) {
			threadlist.push_back(thread(GetTestFunc));
		}

		//Join the threads when they are complete.
		for (std::thread & th : threadlist)
		{
			// If thread Object is Joinable then Join that thread.
			if (th.joinable())
				th.join();
		}
	}

	// Randomly remove some entries
	TEST_F(LRUCacheUnitTests, RemoveTest)
	{
		int NumRemoved = 0;
		const int NumRandomItemsToRemove = 100;
		const auto tmpKeyRange = KeyRange;
		std::unique_ptr<LRUCache<int, string>> cache(new LRUCache<int, string>(tmpKeyRange));

		// Add entries to the Cache
		for(int i=0; i<KeyRange; i++) {
			cache->Put(i,to_string(i));
		}
		ASSERT_EQ(tmpKeyRange, cache->Count());

		for(int i=0; i < NumRandomItemsToRemove; i++)
		{
			if (cache->Remove(rand() % KeyRange)) NumRemoved++;
		}
		cout << "Num Items Removed " << NumRemoved << " matches." << endl;
		ASSERT_EQ(KeyRange-NumRemoved, cache->Count());
	}
#endif
} /* namespace LRUCacheLib */

