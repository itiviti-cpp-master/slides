#pragma once

#include "pool.h"
#include <algorithm>
#include <list>
#include <memory>
#include <ostream>

template <class Key, class T>
class LRU
{
    using Alloc = PoolAllocator<T>;
public:
    LRU(const std::size_t cache_size)
        : m_size(cache_size)
        , m_pool(Alloc::create_pool(cache_size * 2), Alloc::destroy_pool)
        , m_queue(Alloc(*m_pool))
    { }

    std::size_t size() const
    {
        return m_queue.size();
    }

    T & get(const Key & key);

    void debug_dump(std::ostream & strm) const;
private:
    const std::size_t m_size;
    std::unique_ptr<pool::Pool, decltype(&Alloc::destroy_pool)> m_pool;
    std::list<T, Alloc> m_queue;
};

template <class Key, class T>
inline T & LRU<Key, T>::get(const Key & key)
{
    auto it = std::find(m_queue.begin(), m_queue.end(), key);
    if (it != m_queue.end()) {
        while (it != m_queue.begin()) {
            const auto old = it--;
            std::iter_swap(it, old);
        }
    }
    else {
        if (m_size == m_queue.size()) {
            m_queue.pop_back();
        }
        m_queue.emplace_front(key);
    }
    return m_queue.front();
}

template <class Key, class T>
inline void LRU<Key, T>::debug_dump(std::ostream & strm) const
{
    bool first = true;
    for (const auto & x : m_queue) {
        if (!first) {
            strm << " ";
        }
        else {
            first = false;
        }
        strm << x;
    }
    strm << std::endl;
}
