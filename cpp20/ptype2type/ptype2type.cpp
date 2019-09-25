// ptype2type.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <concepts>

template<typename T, size_t N>
struct data2type
{
	T value = 0;

	data2type()
	{}

	template<typename U> requires std::same_as<T, U>
	data2type(U v)
		: value(v)
	{
		std::cout << "basic construct(T v)" << std::endl;
	}

	data2type(data2type<T, N>& o)
		: value(o.value)
	{
		std::cout << "copy construct" << std::endl;
	}

	data2type<T, N>& operator=(T v) {
		value = v;
		std::cout << "operator=(T v)" << std::endl;
		return *this;
	}

};


/*
 * todo : have to find meta auto indexing, alternavtive auto indexing
 */
using mission_id_t = data2type<int, __LINE__>;
using quest_id_t = data2type<int, __LINE__>;

int main()
{
	quest_id_t qid(1); // ok
	quest_id_t qid2 = 2; // ok
	quest_id_t qid3 = qid; // ok 
	//quest_id_t qid4(11ll); // error
	mission_id_t mid(2);
	//mission_id_t mid1(2ll);
	//mid = qid; // error
}

