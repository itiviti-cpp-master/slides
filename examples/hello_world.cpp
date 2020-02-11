#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

int main()
{
    std::string direct, complementary;
    std::cin >> direct;
    complementary.resize(direct.size());
    std::transform(direct.begin(), direct.end(),
                   complementary.begin(),
                   [] (char x) {
                       switch (x) {
                           case 'A': return 'T';
                           case 'T': return 'A';
                           case 'C': return 'G';
                           case 'G': return 'C';
                           default: throw "Bad input";
                       }
                   });
    std::cout << complementary << std::endl;
    return 0;
}

