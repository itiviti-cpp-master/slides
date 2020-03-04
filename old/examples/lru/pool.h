#pragma once

#include <cstdint>
#include <functional>
#include <type_traits>

namespace pool {

class Pool;

Pool * create_pool(std::size_t obj_size, std::size_t obj_count);

void destroy_pool(Pool * pool);

std::size_t pool_obj_size(const Pool & pool);

void * allocate(Pool & pool, std::size_t n);

void deallocate(Pool & pool, void * ptr, std::size_t n);

} // pool namespace

template <class T>
class PoolAllocator
{
    template <class U>
    friend class PoolAllocator;
public:
    static pool::Pool * create_pool(const std::size_t obj_count);
    static void destroy_pool(pool::Pool * pool);

    PoolAllocator(const std::reference_wrapper<pool::Pool> & pool)
        : m_pool(pool)
    { }

    template <class U>
    PoolAllocator(const PoolAllocator<U> & other)
        : m_pool(other.m_pool)
    { }

    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    template <class U>
    struct rebind
    {
        using other = PoolAllocator<U>;
    };

    T * allocate(const std::size_t n);
    void deallocate(T * ptr, const std::size_t n);

    friend bool operator == (const PoolAllocator & lhs, const PoolAllocator & rhs)
    { return lhs.m_pool == rhs.m_pool; }

    friend bool operator != (const PoolAllocator & lhs, const PoolAllocator & rhs)
    { return !(lhs == rhs); }
private:
    std::size_t scale_to_pool(const std::size_t n) const;

    std::reference_wrapper<pool::Pool> m_pool;
};

template <class T>
inline pool::Pool * PoolAllocator<T>::create_pool(const std::size_t obj_count)
{
    return pool::create_pool(sizeof(T), obj_count);
}

template <class T>
inline void PoolAllocator<T>::destroy_pool(pool::Pool * pool)
{
    pool::destroy_pool(pool);
}

template <class T>
inline T * PoolAllocator<T>::allocate(const std::size_t n)
{
    return static_cast<T *>(pool::allocate(m_pool.get(), scale_to_pool(n)));
}

template <class T>
inline void PoolAllocator<T>::deallocate(T * ptr, const std::size_t n)
{
    pool::deallocate(m_pool.get(), ptr, scale_to_pool(n));
}

template <class T>
inline std::size_t PoolAllocator<T>::scale_to_pool(const std::size_t n) const
{
    const std::size_t obj_size = pool_obj_size(m_pool.get());
    if (sizeof(T) <= obj_size) {
        return n;
    }
    else {
        return n * ((sizeof(T) - 1) / obj_size + 1);
    }
}
