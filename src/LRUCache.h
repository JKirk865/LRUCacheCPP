#include <map>
#include <list>
#include <mutex>
#include <list>

using namespace std;

#ifndef LRUCACHE_H_
#define LRUCACHE_H_

namespace LRUCacheLib {

template <class K, class V>
	class LRUCache {
		public:
			LRUCache(size_t Capacity);
			virtual ~LRUCache();

			size_t Capacity() noexcept;
			size_t Count() noexcept;
			void Clear() noexcept;

			optional<V> Get(const K & key);
			void Put(const K & key, const V & value);
			bool Remove(const K & key);

			list<pair<K, V>> ToList()noexcept;

		private:
			size_t    capacity = 0;
			list<K>   cache;
			map<K, V> items;
			mutex     classpadlock;
	};

}; /* namespace LRUCacheLib */

#endif /* LRUCACHE_H_ */
