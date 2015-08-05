#include <iostream>
#include <array>

int GetSize() { return 5; }
constexpr int getSize() { return 5; }

int main()
{
    int arr[getSize()];
    arr[4] = 5;
    std::array<int, getSize()> arr2;

    return 0;


}

