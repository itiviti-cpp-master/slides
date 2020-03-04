#include <algorithm>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class Vector
{
    std::vector<std::string> m_data;
    typename std::vector<std::string>::const_iterator m_end;
public:
    using value_type = std::string;

    Vector(const size_t) {}

    void add(const std::string & str)
    { m_data.push_back(str); }

    void sort()
    {
        std::sort(m_data.begin(), m_data.end());
        m_end = m_data.end();
    }

    void unique()
    { m_end = std::unique(m_data.begin(), m_data.end()); }

    auto begin() const
    { return m_data.begin(); }

    auto end() const
    { return m_end; }
};

class VectorReserve
{
    std::vector<std::string> m_data;
    typename std::vector<std::string>::const_iterator m_end;
public:
    using value_type = std::string;

    VectorReserve(const size_t hint)
    { m_data.reserve(hint > 1000 ? (hint/100) : hint); }

    void add(const std::string & str)
    { m_data.push_back(str); }

    void sort()
    {
        std::sort(m_data.begin(), m_data.end());
        m_end = m_data.end();
    }

    void unique()
    { m_end = std::unique(m_data.begin(), m_data.end()); }

    auto begin() const
    { return m_data.begin(); }

    auto end() const
    { return m_end; }
};

class Deque
{
    std::deque<std::string> m_data;
    typename std::deque<std::string>::const_iterator m_end;
public:
    using value_type = std::string;

    Deque(const size_t) {}

    void add(const std::string & str)
    { m_data.push_back(str); }

    void sort()
    {
        std::sort(m_data.begin(), m_data.end());
        m_end = m_data.end();
    }

    void unique()
    { m_end = std::unique(m_data.begin(), m_data.end()); }

    auto begin() const
    { return m_data.begin(); }

    auto end() const
    { return m_end; }
};

class VectorView
{
    std::vector<char> m_data;
    std::vector<std::string_view> m_views;
    typename std::vector<std::string_view>::const_iterator m_end;
public:
    using value_type = std::string_view;

    VectorView(const size_t hint)
    {
        m_data.reserve(hint);
        m_views.reserve(100 + hint / 100);
    }

    void add(const std::string & str)
    {
        const size_t old_size = m_data.size();
        m_data.insert(m_data.end(), str.begin(), str.end());
        m_views.emplace_back(&m_data[0] + old_size, str.size());
    }

    void sort()
    {
        std::sort(m_views.begin(), m_views.end());
        m_end = m_views.end();
    }

    void unique()
    { m_end = std::unique(m_views.begin(), m_views.end()); }

    auto begin() const
    { return m_views.begin(); }

    auto end() const
    { return m_end; }
};

class Set
{
    std::set<std::string> m_data;
public:
    using value_type = std::string;

    Set(const size_t) {}

    void add(const std::string & str)
    { m_data.insert(str); }

    void sort() {}

    void unique() {}

    auto begin() const
    { return m_data.begin(); }

    auto end() const
    { return m_data.end(); }
};

class MultiSet
{
    std::multiset<std::string> m_data;
public:
    using value_type = std::string;

    MultiSet(const size_t) {}

    void add(const std::string & str)
    { m_data.insert(str); }

    void sort() {}

    void unique() {}

    auto begin() const
    { return m_data.begin(); }

    auto end() const
    { return m_data.end(); }
};

using Lines = VectorView;

template <class C>
void print_out(std::ostream & strm, const C & c, const bool reverse)
{
    std::ostream_iterator<typename C::value_type> out(strm, "\n");
    if (!reverse) {
        std::copy(c.begin(), c.end(), out);
    }
    else {
        std::copy(std::make_reverse_iterator(c.end()),
                std::make_reverse_iterator(c.begin()), out);
    }
}

void sort_stream(std::istream & input, const bool reverse, const bool unique)
{
    // calculate size
    input.seekg(0, std::ios_base::end);
    const auto end_pos = input.tellg();
    input.seekg(0);

    Lines lines(end_pos);

    // read lines
    std::string line;
    while (std::getline(input, line)) {
        lines.add(line);
    }

    lines.sort();

    if (unique) {
        lines.unique();
    }

    print_out(std::cout, lines, reverse);
}

int main(int argc, char ** argv)
{
    bool reverse = false;
    bool unique = false;
    const char * input_name = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] != '-') {
                const size_t len = std::strlen(argv[i]);
                for (size_t j = 1; j < len; ++j) {
                    switch (argv[i][j]) {
                        case 'r':
                            reverse = true;
                            break;
                        case 'u':
                            unique = true;
                            break;
                    }
                }
            }
            else {
                if (std::strcmp(argv[i], "--reverse") == 0) {
                    reverse = true;
                }
                else if (std::strcmp(argv[i], "--unique") == 0) {
                    unique = true;
                }
            }
        }
        else {
            input_name = argv[i];
        }
    }

    if (input_name != nullptr) {
        std::ifstream f(input_name);
        sort_stream(f, reverse, unique);
    }
    else {
        sort_stream(std::cin, reverse, unique);
    }
}
