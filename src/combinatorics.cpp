﻿#include <kbd/combinatorics.h>

namespace kbd
{

//-----------------------------------------------------------------------------
Number::Number(int base, int count) : m_mas(count, 0), m_base(count, base), m_end(false) {
}

//-----------------------------------------------------------------------------
Number::Number(const std::vector<int>& base) : m_mas(base.size(), 0), m_base(base), m_end(false) {
}

//-----------------------------------------------------------------------------
Number& Number::operator++(int) {
	m_mas[0]++;
	int current = 0;
	while (current < m_base.size() && m_mas[current] >= m_base[current]) {
		m_mas[current] = 0;
		current++;
		if (current < m_base.size())
			m_mas[current]++;
	}
	if (current == m_base.size())
		m_end = true;
	return *this;
}

//-----------------------------------------------------------------------------
bool Number::isEnd(void) const {
	return m_end;
}

//-----------------------------------------------------------------------------
std::vector<int> Number::get(void) const {
	return m_mas;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Compositions::Compositions(int sum) : m_num(2, sum-1) {
}

//-----------------------------------------------------------------------------
Compositions& Compositions::operator++(int) {
	m_num++;
	return *this;
}

//-----------------------------------------------------------------------------
bool Compositions::isEnd(void) const {
	return m_num.isEnd();
}

//-----------------------------------------------------------------------------
std::vector<int> Compositions::get(void) const {
	std::vector<int> composition;
	auto mas = m_num.get();
	bool isAllZero = true;
	for (int i = 0; i < mas.size(); ++i) {
		if (mas[i] == 0) {
			composition.push_back(1);
		} else {
			isAllZero &= false;
			int count = 0;
			while (i < mas.size() && mas[i] == 1) {
				count++;
				i++;
			}
			composition.push_back(count+1);
		}
	}

	if (mas.back() != 1)
		composition.push_back(1);

	return composition;
}

};