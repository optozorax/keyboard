﻿#include <iostream>

#include <kbd/combinatorics.h>

int main() {
	using namespace kbd;

	//-------------------------------------------------------------------------
	Number num({2, 3, 2, 4});
	do {
		auto result = num.get();
		for (int i = 0; i < result.size(); i++)
			std::cout << result[result.size() - i - 1];
		std::cout << std::endl;
		num++;
	} while (!num.isEnd());

	std::cout << std::endl;

	//-------------------------------------------------------------------------
	Compositions comp(6);
	do {
		auto result = comp.get();
		int sum = 0;
		for (auto& i : result) {
			std::cout << i;
			sum += i;
		}
		std::cout << " = " << sum << std::endl;
		comp++;
	} while (!comp.isEnd());

	system("pause");
}