#pragma once

#include <tuple>
#include <concepts>

class component
{

};


class ac : public component
{
public:
	void print() { std::cout << "print ac!!" << std::endl; }
};

class bc : public component
{
public:
	void print() { std::cout << "print bc!!" << std::endl; }
};

class cc : public component
{
public:
	void print() { std::cout << "print cc!!" << std::endl; }
};

template<class T>
concept derived_from_component = std::derived_from<T, component>;


class component_owner
{
	std::tuple<ac, bc, cc> _components;

public:
	// case 1
	/*template<std::derived_from<component> T>
	T* get() { return &std::get<T>(_components); }*/

	// case 2
	/*template<typename T> requires std::derived_from<T, component>
	T* get() { return &std::get<T>(_components); }*/

	// case 3
	template<derived_from_component T>
	T* get() { return &std::get<T>(_components); }






	
};

