#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <stack>


template <typename T>
auto PalindromCheck(T container){
    auto isPalindrom {
            [](std::string str) {
                for (int i = 0; i < str.size(); i++) {
                    if (str[i] != str[str.size() - 1 - i]) {return false;}
                }
                return true;
            }
    };

    for (auto ptr : container) {
        if (isPalindrom(ptr)) {std::cout << ptr << std::endl;}
    }
};

int main() {
    std::set<std::string> s;
    s.insert("abracadabra");
    s.insert("abracarba");
    PalindromCheck(s);

    std::vector<std::string> v;
    for (int i = 0; i < 40; i++) {
        v.push_back(std::to_string(i));
    }
    PalindromCheck(v);
    return 0;
}

