#include <iostream>
#include <string>
#include <vector>

int main() {
    int *a = new int(1);
    std::vector<int *> v;
    v.push_back(a);
    v.push_back(a);

    int *b = std::move(v.front());
    printf("%d\n", v.size());    
}