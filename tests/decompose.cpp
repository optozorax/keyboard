﻿#include <iostream>

#include <kbd/keyboard.h>
#include <kbd/combinatorics.h>

std::vector<Keyboard::KeyboardKey> zergox = {
	/** Улучшенный вариант ErgoDox-EZ: zergox (zorax + ergo).
		В этой клавиатуре не показаны другие дополнительные клавиши, только буквенные.
		Количество клавиш: 56.
		Используются все пальцы: от мизинца до большого.
		На каждый палец по 4 клавиши, на мизинец и указательный по 8.
	*/
	/*------------------------------ЛЕВАЯ РУКА-------------------------------*/
	// Боковая часть мизинца
	{0, 0, 1.5, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_HIGHEST, COLUMN_LEFT},
	{0, 1, 1.5, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_UPPER, COLUMN_LEFT},
	{0, 2, 1.5, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_MIDDLE, COLUMN_LEFT},
	{0, 3, 1.5, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_LOWER, COLUMN_LEFT},

	// Основная часть мизинца
	{1, 0, 1, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_HIGHEST, COLUMN_MIDDLE},
	{1, 1, 1, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_UPPER, COLUMN_MIDDLE},
	{1, 2, 1, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_MIDDLE, COLUMN_MIDDLE},
	{1, 3, 1, 1, 0, HAND_LEFT, FINGER_PINKY, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть безымянного
	{2, 0, 1, 1, 0, HAND_LEFT, FINGER_ANNULAR, ROW_HIGHEST, COLUMN_MIDDLE},
	{2, 1, 1, 1, 0, HAND_LEFT, FINGER_ANNULAR, ROW_UPPER, COLUMN_MIDDLE},
	{2, 2, 1, 1, 0, HAND_LEFT, FINGER_ANNULAR, ROW_MIDDLE, COLUMN_MIDDLE},
	{2, 3, 1, 1, 0, HAND_LEFT, FINGER_ANNULAR, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть среднего
	{3, 0, 1, 1, 0, HAND_LEFT, FINGER_MIDDLE, ROW_HIGHEST, COLUMN_MIDDLE},
	{3, 1, 1, 1, 0, HAND_LEFT, FINGER_MIDDLE, ROW_UPPER, COLUMN_MIDDLE},
	{3, 2, 1, 1, 0, HAND_LEFT, FINGER_MIDDLE, ROW_MIDDLE, COLUMN_MIDDLE},
	{3, 3, 1, 1, 0, HAND_LEFT, FINGER_MIDDLE, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть указательного
	{4, 0, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_HIGHEST, COLUMN_MIDDLE},
	{4, 1, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_UPPER, COLUMN_MIDDLE},
	{4, 2, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_MIDDLE, COLUMN_MIDDLE},
	{4, 3, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_LOWER, COLUMN_MIDDLE},

	// Боковая часть указательного
	{5, 0, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_HIGHEST, COLUMN_RIGHT},
	{5, 1, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_UPPER, COLUMN_RIGHT},
	{5, 2, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_MIDDLE, COLUMN_RIGHT},
	{5, 3, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_LOWER, COLUMN_RIGHT},

	// Основная часть большого
	{8.5, 4, 1, 1.5, 0, HAND_LEFT, FINGER_THUMB, ROW_HIGHEST, COLUMN_MIDDLE},
	{7.5, 4, 1, 1.5, 0, HAND_LEFT, FINGER_THUMB, ROW_UPPER, COLUMN_MIDDLE},
	{6.5, 4, 1, 1.5, 0, HAND_LEFT, FINGER_THUMB, ROW_MIDDLE, COLUMN_MIDDLE},
	{5.5, 4, 1, 1.5, 0, HAND_LEFT, FINGER_THUMB, ROW_LOWER, COLUMN_MIDDLE},

	/*------------------------------ПРАВАЯ РУКА------------------------------*/
	// Основная часть большого
	{10.5, 4, 1, 1.5, 0, HAND_RIGHT, FINGER_THUMB, ROW_HIGHEST, COLUMN_MIDDLE},
	{11.5, 4, 1, 1.5, 0, HAND_RIGHT, FINGER_THUMB, ROW_UPPER, COLUMN_MIDDLE},
	{12.5, 4, 1, 1.5, 0, HAND_RIGHT, FINGER_THUMB, ROW_MIDDLE, COLUMN_MIDDLE},
	{13.5, 4, 1, 1.5, 0, HAND_RIGHT, FINGER_THUMB, ROW_LOWER, COLUMN_MIDDLE},

	// Боковая часть указательного
	{13, 0, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_HIGHEST, COLUMN_LEFT},
	{13, 1, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_UPPER, COLUMN_LEFT},
	{13, 2, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_MIDDLE, COLUMN_LEFT},
	{13, 3, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_LOWER, COLUMN_LEFT},

	// Основная часть указательного
	{14, 0, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_HIGHEST, COLUMN_MIDDLE},
	{14, 1, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_UPPER, COLUMN_MIDDLE},
	{14, 2, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_MIDDLE, COLUMN_MIDDLE},
	{14, 3, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть среднего
	{15, 0, 1, 1, 0, HAND_RIGHT, FINGER_MIDDLE, ROW_HIGHEST, COLUMN_MIDDLE},
	{15, 1, 1, 1, 0, HAND_RIGHT, FINGER_MIDDLE, ROW_UPPER, COLUMN_MIDDLE},
	{15, 2, 1, 1, 0, HAND_RIGHT, FINGER_MIDDLE, ROW_MIDDLE, COLUMN_MIDDLE},
	{15, 3, 1, 1, 0, HAND_RIGHT, FINGER_MIDDLE, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть безымянного
	{16, 0, 1, 1, 0, HAND_RIGHT, FINGER_ANNULAR, ROW_HIGHEST, COLUMN_MIDDLE},
	{16, 1, 1, 1, 0, HAND_RIGHT, FINGER_ANNULAR, ROW_UPPER, COLUMN_MIDDLE},
	{16, 2, 1, 1, 0, HAND_RIGHT, FINGER_ANNULAR, ROW_MIDDLE, COLUMN_MIDDLE},
	{16, 3, 1, 1, 0, HAND_RIGHT, FINGER_ANNULAR, ROW_LOWER, COLUMN_MIDDLE},

	// Основная часть мизинца
	{17, 0, 1, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_HIGHEST, COLUMN_MIDDLE},
	{17, 1, 1, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_UPPER, COLUMN_MIDDLE},
	{17, 2, 1, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_MIDDLE, COLUMN_MIDDLE},
	{17, 3, 1, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_LOWER, COLUMN_MIDDLE},

	// Боковая часть мизинца
	{18, 0, 1.5, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_HIGHEST, COLUMN_RIGHT},
	{18, 1, 1.5, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_UPPER, COLUMN_RIGHT},
	{18, 2, 1.5, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_MIDDLE, COLUMN_RIGHT},
	{18, 3, 1.5, 1, 0, HAND_RIGHT, FINGER_PINKY, ROW_LOWER, COLUMN_RIGHT},
};

int main() {
	using namespace kbd;

	std::vector<Keyboard::KeyboardKey> keyboard4 = {
		{0, 0, 1, 1, 0, HAND_LEFT, FINGER_INDEX, ROW_MIDDLE, COLUMN_MIDDLE},
		{1, 0, 1, 1, 0, HAND_LEFT, FINGER_THUMB, ROW_MIDDLE, COLUMN_MIDDLE},
		{3, 0, 1, 1, 0, HAND_RIGHT, FINGER_THUMB, ROW_MIDDLE, COLUMN_MIDDLE},
		{4, 0, 1, 1, 0, HAND_RIGHT, FINGER_INDEX, ROW_MIDDLE, COLUMN_MIDDLE},
	};

	std::vector<Layout::LayoutSymbols> layout24 = {
		{{0, 0}, L"a"},
		{{0, 1}, L"①"},
		{{0, 2}, L" "},
		{{0, 3}, L". ①"},

		{{1, 0}, L"b"},
		{{1, 1}, L"①"},
		{{1, 2}, L","},
		{{1, 3}, L"."},
	};

	std::vector<Layout::LayoutSymbols> layout44 = {
		{{0, 0}, L"a"},
		{{0, 1}, L"①"},
		{{0, 2}, L" "},
		{{0, 3}, L". "},

		{{1, 0}, L"b"},
		{{1, 1}, L""},
		{{1, 2}, L"②"},
		{{1, 3}, L"."},

		{{2, 0}, L"c"},
		{{2, 1}, L""},
		{{2, 2}, L""},
		{{2, 3}, L"③"},

		{{2, 0}, L"d"},
		{{2, 1}, L""},
		{{2, 2}, L""},
		{{2, 3}, L""},	
	};

	Keyboard keyboard("test", keyboard4);
	Layout layout(keyboard, layout24);

	//std::wstring text1 = L"a. a. b."; 
	std::wstring text1 = L"a.b, ab babaaa, ,. baba bb,. a . b";

	// Составляем массив сколько возможных вариантов есть, чтобы набрать тот или иной символ.
	std::vector<int> variants(text1.size(), 0);
	for (int i = 0; i < text1.size(); ++i)
		variants[i] = layout.getKeys(text1[i]).size();

	std::vector<Keys> allVariants;

	Number num(variants);
	do {
		auto res = num.get();

		// Формируем какие строки будут после каждого нажатия
		std::vector<std::wstring> strings(text1.size());
		std::vector<Key> keys(text1.size());
		for (int i = 0; i < text1.size(); ++i) {
			keys[i] = layout.getKeys(text1[i])[res[i]];
			strings[i] = layout.getSymbols(keys[i]);
		}

		// Убираем лишние части, если есть строки с несколькими символами, если они подходят, если не подходят, то этот вариант проигрышный. Так же здесь фильтруем автоматическое переключение слоя
		int pos = 0;
		for (int i = 0; i < strings.size(); ++i) {
			if (strings[i].size() != 1) {
				// Проверяем, чтобы дальнейший текст соответствовал тому, что автоматически наберется в клавише с несколькими символами
				for (int j = 0; j < strings[i].size(); ++j) {
					if (!getLayer(strings[i][j]))
						if (pos + j < text1.size()) {
							if (text1[pos + j] != strings[i][j])
								goto next_variant;
						} else
							goto next_variant;
				}

				// Удаляем все варианты, которые идут после клавиш с несколькими символами
				for (int j = 1; j < strings[i].size(); ++j) {
					if (!getLayer(strings[i][j]) && i < strings.size() - 1) {
						strings.erase(strings.begin() + i + 1);
						keys.erase(keys.begin() + i + 1);
					} 
				}
			}

			// Если в клавише есть переключение следующего слоя, то следующая клавиша должна быть на этом слое
			auto nextLayer = getLayer(strings[i].back());
			if (nextLayer && i+1 < keys.size() && keys[i+1].layer != *nextLayer)
				goto next_variant;

			pos += strings[i].size();
			if (getLayer(strings[i].back()))
				pos--;
		}

		// Формируем массив следующих клавиш, где записывается информация касательно слоя
		allVariants.push_back(keys);

		next_variant:
		num++;
	} while (!num.isEnd());

	system("pause");
}

	/*
	Имеется текущий слой
	Перебираем все символы
		Находим для каждого символа сколько существует вариантов набора
	Перебираем все эти нажатия
		Для всех нажатий, что дают несколько символов проверяем соответствуют ли они тексту далее, и удаляем дальнейшие символы, так как они уже наберутся за счет предыдущей клавиши

		Затем закидываем все эти клавиши в массив. Там же туда закидываем отдельно не клавишные события, которые переключают слой. Далее находим все клавиши длиной более 3, чтобы их объединить в зажатую клавишу слоя
		Помещаем туда клавишу для переключения слоя

	Разложение большого текста на оптимальный метод набора
		Берется часть текста
		Эта часть разбивается на клавиши, которые необхоимо нажать, эти все клавиши могут находиться на разных слоях. Key
		Этот результат дополняется нажатием клавиш для включения слоёв. KeyPos
		Все эти варианты набора текста проверяются в раскладке.
		Нажатия клавиш на клавиатуре разбиваются на аккорды. Accords
		Все эти аккорды оцениваются, и находится оптимальный метод набора, он записывается в массив нажатий
		Часть текста, которая набирается оптимальным образом обрезается от текста
		Если текст не кончился, то возвращаемся назад.
	*/