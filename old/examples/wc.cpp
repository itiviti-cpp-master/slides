#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

namespace {

struct Stats
{
    size_t lines = 0;
    size_t words = 0;
    size_t bytes = 0;
    size_t characters = 0;
    size_t max_line_length = 0;

    void print() const
    {
        std::cout << lines << "\t" << words << "\t" << characters << "\t" << bytes << "\t" << max_line_length << std::endl;
    }
};

template <class It>
void advance(It & it, const size_t n, const It end)
{
    it += std::min<size_t>(n, end-it);
}

size_t count_characters(const std::string & str)
{
    size_t characters = 0;
    for (auto it = str.begin(), end = str.end(); it != end; ++characters) {
        if ((*it & 0x80) == 0) {
            ++it;
        }
        else if ((*it & 0xE0) == 0xC0) {
            advance(it, 2, end);
        }
        else if ((*it & 0xF0) == 0xE0) {
            advance(it, 3, end);
        }
        else if ((*it & 0xF8) == 0xF0) {
            advance(it, 4, end);
        }
    }
    return characters;
}

size_t count_words(const std::string & str)
{
    size_t words = 0;
    auto it = str.begin();
    const auto end = str.end();
    while (it != end && std::isspace(*it)) {
        ++it;
    }
    while (it != end) {
        auto next = it + 1;
        while (next != end && !std::isspace(*next)) {
            ++next;
        }
        while (next != end && std::isspace(*next)) {
            ++next;
        }
        ++words;
        it = next;
    }
    return words;
}

Stats process_stream(std::istream & input)
{
    Stats stats;
    std::string line("foo");
    while (std::getline(input, line)) {
        ++stats.lines;
        stats.words += count_words(line);
        stats.bytes += line.size() + 1;
        const size_t line_characters = count_characters(line);
        stats.characters += line_characters + 1;
        stats.max_line_length = std::max(stats.max_line_length, line_characters);
    }
    if (stats.lines > 0 && !line.empty()) {
        --stats.lines;
        --stats.bytes;
        --stats.characters;
    }
    return stats;
}

} // anonymous namespace

int main()
{
    const auto stats = process_stream(std::cin);
    stats.print();
}

