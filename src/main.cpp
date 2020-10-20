#include <list>
#include <iostream>
#include <string>
#include "LRUCache.h"
#include "LRUCacheUnitTests.h"
using namespace LRUCacheLib;


/**
 * This main() runs some unittest and timing code from the LRUCacheUnitTests class.
 */
int main()
{
	cout << " LRUCache Tests" << endl;

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

	cout << "new LRUCache Tests Finished" << endl;
	return 0;
}



