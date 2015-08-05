#include <iostream>

#include <map>

struct Data
{
    int a = 0;
    int b = 0;
};

typedef std::tuple<int, int, int> Key;
typedef std::map<Key, Data> Datas;

int main()
{
    Datas datas;
    datas.insert( Datas::value_type(std::make_tuple(1,2,3), Data()));

    auto i = datas.find(Key(1,2,3));
    if ( i != datas.end())
    {
        std::cout << "found\n" << std::endl;
    }



}

