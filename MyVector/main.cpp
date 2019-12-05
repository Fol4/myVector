#include <iostream>
#include "myvector.h"

int main()
{
	myvector mv(10);
	for (int i = 0; i < 10; ++i)
	{
		mv.set(i, i);
		std::cout << mv.get(i) << " ";
	}

	std::cout << std::endl;

	for (int i = 1; i < 10; ++i)
	{
		mv.set(i, mv.get(i) + mv.get(i - 1));
		std::cout << mv.get(i) << " ";
	}
}