#include <list>
#include <iostream>
#include <string>
#include "LRUCache.h"

#ifndef LRUCACHEUNITTESTS_H_
#define LRUCACHEUNITTESTS_H_

using namespace std;

namespace LRUCacheLib {

class LRUCacheUnitTests
{
	public:
	  static void DumpList(list<pair<int, string>> l, string title);
	  static void PutTest(LRUCache<int, string> &cache);
	  static void GetTest(LRUCache<int, string> &cache);
	  static void RemoveTest(LRUCache<int, string> &cache, int NumRandomItemsToRemove=1000, int KeyRange = 1000);
};

} /* namespace LRUCacheLib */

#endif /* LRUCACHEUNITTESTS_H_ */
