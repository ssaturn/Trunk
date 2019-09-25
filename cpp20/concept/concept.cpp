// concept.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <concepts>
#include "component.h"

namespace contents
{
	class player : public component_owner
	{

	};
}

int main()
{
	auto player = new contents::player();
	auto acompoent = player->get<ac>();
	acompoent->print();

	auto bcompoent = player->get<bc>();
	bcompoent->print();

	auto ccompoent = player->get<cc>();
	ccompoent->print();

}

