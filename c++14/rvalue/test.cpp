#include <iostream>
#include <utility>


//void print(int && value)
//{
//    printf("call rvalue\n");
//}

void print(const int& value)
{
    printf("call lvalue\n");
}

int main()
{
    int a = 1024;
    print(a);
    print(std::move(a));
    return 0;
}

