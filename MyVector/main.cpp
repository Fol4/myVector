#include <iostream>
#include "myvector.h"
#include <cmath>

int main()
{
	myvector v1{ 1,3,5,7,9, };
	myvector v2{ 2,4,6,8,10 };
	std::cout << v2.size() << " " << v1.size() << std::endl;
	v2 = v1;
	std::cout << "v1: ";
	for (unsigned long long i = 0; i < v1.size(); ++i)
		std::cout << v1[i] << " ";
	std::cout << std::endl;
	std::cout << "v2: ";
	for (unsigned long long i = 0; i < v2.size(); ++i)
		std::cout << v2[i] << " ";
}