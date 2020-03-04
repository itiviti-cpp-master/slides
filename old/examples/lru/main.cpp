#include "lru.h"

#include <iostream>
#include <string>

namespace {

struct String
{
    int key;
    std::string data;

    String(const int key_)
        : key(key_)
        , data(std::to_string(key_))
    { }

    bool operator == (int other_key)
    { return key == other_key; }

    bool operator != (int other_key)
    { return key != other_key; }

    friend std::ostream & operator << (std::ostream & strm, const String & s)
    { return strm << s.key << "[" << s.data << "]"; }
};

using Cache = LRU<int, String>;

bool test()
{
#define EXPECT(x, msg) \
    do { \
        if (!(x)) { \
            std::cerr << msg << " at " << __FILE__ << "(" << __LINE__ << ")" << std::endl; \
            return false; \
        } \
    } while (false)

    Cache cache(10);

    auto & s1 = cache.get(1);
    EXPECT(s1.data == "1", "Wrong item 1: " << s1.data);
    auto & s2 = cache.get(2);
    EXPECT(s2.data == "2", "Wrong item 2: " << s2.data);
    auto & s1_a = cache.get(1);
    EXPECT(s1_a.data == "1", "Wrong item 1 (a): " << s1_a.data);
    EXPECT(cache.size() == 2, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

    for (int i = 0; i < 10; ++i) {
        auto & s = cache.get(i);
        EXPECT(s.data == std::to_string(i), "Wrong item " << i << ": " << s.data);
        s.data += '@';
    }
    EXPECT(cache.size() == 10, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

    for (int i = 0; i < 10; ++i) {
        auto & s = cache.get(i);
        EXPECT(s.data == (std::to_string(i) + '@'), "Wrong item " << i << ": " << s.data);
    }
    EXPECT(cache.size() == 10, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

    for (int i = 0; i < 30; ++i) {
        auto & s = cache.get(i);
        s.data += '!';
    }
    EXPECT(cache.size() == 10, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

    auto & s23 = cache.get(23);
    EXPECT(s23.data == "23!", "Wrong item 23: " << s23.data);
    auto & s26 = cache.get(26);
    EXPECT(s26.data == "26!", "Wrong item 26: " << s26.data);
    cache.debug_dump(std::cout);

    for (int i = 29; i > 19; --i) {
        auto &s = cache.get(i);
        EXPECT(s.data == (std::to_string(i) + '!'), "Wrong item " << i << ": " << s.data);
    }
    EXPECT(cache.size() == 10, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

    for (int i = 10; i < 20; ++i) {
        auto &s = cache.get(i);
        EXPECT(s.data == std::to_string(i), "Wrong item " << i << ": " << s.data);
    }
    EXPECT(cache.size() == 10, "Wrong cache size: " << cache.size());
    cache.debug_dump(std::cout);

#undef EXPECT
    return true;
}

} // anonymous namespace

int main()
{
    const auto result = test();
    std::cout << "Test passed: " << std::boolalpha << result << std::endl;
}
