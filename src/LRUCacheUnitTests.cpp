#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "LRUCache.h"
#include "LRUCacheUnitTests.h"

using namespace std;

namespace LRUCacheLib {

	void LRUCacheUnitTests::DumpList(list<pair<int, string>> l, string title)
	{
		cout << title << endl;
		cout << "Oldest to Newest: " << l.size() << "." << endl;
		for(auto i : l) {
			cout << "Key:" << i.first << " Value:" << i.second << endl;
		}
	}

	void LRUCacheUnitTests::PutTest(LRUCache<int, string> &cache)
	{
	    const auto NumPutsPerThread = 1'000;
	    const auto NumThreads = 10;

	    try {
			// Create a Lambda function that add unique elements to the cache
			auto PutTestFunc = [](
					LRUCacheLib::LRUCache<int, string> *cache,
					int NumPuts, int StartInt) {
				for(int i=StartInt; i<(StartInt+NumPuts); i++) {
					cache->Put(i,to_string(i));
				}
			};

			//Create a list of threads that will each call the PutTestFunc asynchronously.
			vector<thread> threadlist;
			auto t1 = std::chrono::high_resolution_clock::now();
			//Create and Start the Threads
			for(int i=0; i < NumThreads; i++) {
				threadlist.push_back(thread(PutTestFunc, &cache, NumPutsPerThread, (i*NumPutsPerThread)+1));
			}
			//Join the threads when they are complete.
			for (std::thread & th : threadlist)
			{
				// If thread Object is Joinable then Join that thread.
				if (th.joinable())
					th.join();
			}
			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
			cout << "PutTest duration: " << duration << " microSeconds." << endl;
			threadlist.clear(); // Clear the Vector to ensure the threads are destroyed.
	    } catch(exception &e)
	    {
	    	cerr << "Exception thrown in PutTest(). " << e.what() <<  endl;
	    }
	}

	void LRUCacheUnitTests::GetTest(LRUCache<int, string> &cache)
	{
	    const auto NumGetsPerThread = 1'000;
	    const auto NumThreads = 10;
	    const auto KeyRange = 10'000;

	    try {
			cout << "Starting GetTest() with " << cache.Count() << " entries." << endl;

	    	// Create a Lambda function that attempt to find random elements from the cache
			auto GetTestFunc = [](
					LRUCacheLib::LRUCache<int, string> *cache,
					int NumGets, int KeyRange) {
				int NumFound = 0;
				for(int i=0; i<NumGets; i++) {
					auto v = cache->Get(rand() % KeyRange);
					if (v.has_value()) NumFound++;
				}
				cout << "GetTest Thread located " << NumFound << " matches." << endl;
			};

			//Create a list of threads that will each call the GetTestFunc asynchronously.
			vector<thread> threadlist;
			auto t1 = std::chrono::high_resolution_clock::now();
			//Create and Start the Threads
			for(int i=0; i < NumThreads; i++) {
				threadlist.push_back(thread(GetTestFunc, &cache, NumGetsPerThread, KeyRange));
			}
			//Join the threads when they are complete.
			for (std::thread & th : threadlist)
			{
				// If thread Object is Joinable then Join that thread.
				if (th.joinable())
					th.join();
			}
			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
			cout << "GetTest duration: " << duration << " microSeconds." << endl;
			threadlist.clear(); // Clear the Vector to ensure the threads are destroyed.
	    } catch(exception &e)
	    {
	    	cerr << "Exception thrown in GetTest(). " << e.what() <<  endl;
	    }
	}

	void LRUCacheUnitTests::RemoveTest(LRUCache<int, string> &cache, int NumRandomItemsToRemove, int KeyRange)
	{
		int NumRemoved = 0;
		cout << "Starting RemoveTest() with " << cache.Count() << " entries." << endl;
		for(int i=0; i < NumRandomItemsToRemove; i++)
		{
			if (cache.Remove(rand() % KeyRange)) NumRemoved++;
		}
		cout << "Num Items Removed " << NumRemoved << " matches." << endl;
		cout << "Finished RemoveTest() with " << cache.Count() << " entries." << endl;
	}
} /* namespace LRUCacheLib */
