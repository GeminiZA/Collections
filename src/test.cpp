#include "Collections.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace Collections;

int main()
{
    Vector<int> v = Vector<int>();
    v.push_back(1);
    v.push_back(2);
    auto v2 = v.map([](int a) -> int
                    { return a * 2; })
                  .toList();
    std::cout << v.toString() << std::endl;
    std::cout << v2.toString() << std::endl;
    List<int> l = v.toList();
    List l2 = l.map([](int a) -> int
                    { return a * a; });
    std::cout << l.toString() << std::endl;
    std::cout << l2.toString() << std::endl;
}