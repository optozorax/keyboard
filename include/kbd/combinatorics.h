#pragma once

#include <vector>

namespace kbd
{

	//-------------------------------------------------------------------------
	/** Число на основе какой-либо системы счисления. Система счисления может быть переменной для каждой цифры. */
	class Number
	{
	public:
		Number(int base, int count);
		Number(const std::vector<int>& base);

		Number& operator++(int);
		bool isEnd(void) const;

		std::vector<int> get(void) const;
	private:
		std::vector<int> 	m_mas;
		std::vector<int> 	m_base;
		bool				m_end;
	};

	//-------------------------------------------------------------------------
	/** Перебор всех возможных сумм композиций числа. 
		Пример: все композиции числа 5
		    5
			4 + 1
			3 + 2
			3 + 1 + 1
			2 + 3
			2 + 2 + 1
			2 + 1 + 2
			2 + 1 + 1 + 1
			1 + 4
			1 + 3 + 1
			1 + 2 + 2
			1 + 2 + 1 + 1
			1 + 1 + 3
			1 + 1 + 2 + 1
			1 + 1 + 1 + 2
			1 + 1 + 1 + 1 + 1
	 */
	class Compositions
	{
	public:
		Compositions(int sum);

		Compositions& operator++(int);
		bool isEnd(void) const;

		std::vector<int> get(void) const;
	private:
		Number m_num;
	};

};