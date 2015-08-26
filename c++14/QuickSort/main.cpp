#include <iostream>


#include <vector>

void Swap(int& lhs, int& rhs)
{
    int temp = lhs;
    lhs = rhs;
    rhs = temp;
}

int Partition(std::vector<int>& v, int left, int right)
{
    int first = left;
    int pivot = v[first];
    ++left;

    while ( left <= right )
    {
        while ( v[left] <= pivot && left < right ) {
            ++left;
        }

        while ( v[right] >= pivot && left <= right ) {
            --right;
        }

        if ( left < right )
            Swap( v[left], v[right]);
        else
            break;

    }

    Swap( v[first], v[right]);

    for ( auto& e : v ) printf("%d ", e);
    printf("\n");

    return right;
}

void QuickSort(std::vector<int>& v, int lhs, int rhs)
{
    printf("step 2\n");
    if (lhs < rhs)
    {
        int i = Partition(v, lhs, rhs);
        QuickSort(v, lhs, i - 1 );
        QuickSort(v, i + 1, rhs);
    }

}



int main()
{
    printf("1\n");
    std::vector<int> v = { 1,3,2,5,8,19,9,20 };
    QuickSort(v, 0, v.size()-1);
    for ( auto& e : v )
    {
        printf("%d ", e);
    }

    printf("\n");

    return 0;
}
