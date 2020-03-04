#include "pool.h"

#include <assert.h>
#include <cstddef>
#include <new>
#include <vector>

using std::size_t;

namespace pool {

class Pool
{
public:
    Pool(const size_t obj_size, const size_t obj_count)
        : m_obj_size(obj_size)
        , m_storage(obj_size * obj_count)
        , m_used_map(obj_count)
    { }

    size_t get_obj_size() const
    {
        return m_obj_size;
    }

    void * allocate(size_t n);

    void deallocate(void * ptr, size_t n);

private:
    static constexpr size_t npos = static_cast<size_t>(-1);

    size_t find_empty_place(size_t n) const;

    const size_t m_obj_size;
    std::vector<std::byte> m_storage;
    std::vector<bool> m_used_map;
};

size_t Pool::find_empty_place(const size_t n) const
{
    if (n > m_used_map.size()) {
        return npos;
    }
    for (size_t i = 0; i < m_used_map.size(); ++i) {
        if (m_used_map[i]) {
            continue;
        }
        size_t j = i;
        for (size_t k = 0; k < n && j < m_used_map.size(); ++k, ++j) {
            if (m_used_map[j]) {
                break;
            }
        }
        if (n == j - i) {
            return i;
        }
        i = j;
    }
    return npos;
}

void * Pool::allocate(const size_t n)
{
    const size_t pos = find_empty_place(n);
    if (pos != npos) {
        for (size_t i = pos, end = pos + n; i < end; ++i) {
            m_used_map[i] = true;
        }
        return &m_storage[pos * m_obj_size];
    }
    throw std::bad_alloc{};
}

void Pool::deallocate(void * ptr, const size_t n)
{
    auto b_ptr = static_cast<const std::byte *>(ptr);
    const auto begin = &m_storage[0];
    if (b_ptr >= begin) {
        const size_t offset = (b_ptr - begin) / m_obj_size;
        assert(((b_ptr - begin) % m_obj_size) == 0);
        if (offset < m_used_map.size()) {
            const size_t end_delete = offset + std::min(n, m_used_map.size() - offset);
            for (size_t i = offset; i < end_delete; ++i) {
                m_used_map[i] = false;
            }
        }
    }
}

Pool * create_pool(const size_t obj_size, const size_t obj_count)
{
    return new Pool(obj_size, obj_count);
}

void destroy_pool(Pool * pool)
{
    delete pool;
}

size_t pool_obj_size(const Pool & pool)
{
    return pool.get_obj_size();
}

void * allocate(Pool & pool, const size_t n)
{
    return pool.allocate(n);
}

void deallocate(Pool & pool, void * ptr, const size_t n)
{
    pool.deallocate(ptr, n);
}

} // pool namespace
