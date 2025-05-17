#include "SharedPtr.hpp"

#include <cassert>
#include <iostream>
#include <vector>

struct Yoink {
    Yoink(SharedPtr<int> ptr) : p(ptr) {}
    ~Yoink() {}
    SharedPtr<int> p;
};

struct Aggregate {
    Aggregate(const std::string& s, int i, std::vector<int> v)
        : s(s), i(i), v(v) {}
    std::string s;
    int i;
    std::vector<int> v;
};

void copies_galore() {
    SharedPtr<int> p(new int(420));

    Yoink copy1(p);
    SharedPtr<int> copy2(p);
    SharedPtr<int> copy3 = copy2;

    if (p) {
        std::cout << "p contains:\t" << *p << std::endl;
    }
}

void aggregate() {
    int data[] = {1, 2, 3};
    std::size_t sz = sizeof(data) / sizeof(*data);
    SharedPtr<Aggregate> a(
        new Aggregate("abc", 420, std::vector<int>(data, data + sz)));

    assert(a->s == "abc");
    assert(a->i == 420);
    assert(a->v.at(0) == 1);
    assert(a->v.at(1) == 2);
    assert(a->v.at(2) == 3);
}

int main() {
    copies_galore();
}
