#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <kbd/keyboard.h>

/** Тестируется:
	getSymbols
	getKeys
	getLayerKeys
	typeTaps
	typeKeys
 */

using namespace kbd;

std::vector<Keyboard::KeyboardKey> tenkeyKeys = {
	/*-----------------------------------------------------------------------*/
	/*------------------------------ЛЕВАЯ РУКА-------------------------------*/
	{0, 0, 1, 1, 0,  HAND_LEFT,  FINGER_PINKY,   ROW_MIDDLE, COLUMN_MIDDLE},
	{1, 0, 1, 1, 0,  HAND_LEFT,  FINGER_ANNULAR, ROW_MIDDLE, COLUMN_MIDDLE},
	{2, 0, 1, 1, 0,  HAND_LEFT,  FINGER_MIDDLE,  ROW_MIDDLE, COLUMN_MIDDLE},
	{3, 0, 1, 1, 0,  HAND_LEFT,  FINGER_INDEX,   ROW_MIDDLE, COLUMN_MIDDLE},
	{4, 0, 1, 1, 0,  HAND_LEFT,  FINGER_THUMB,   ROW_MIDDLE, COLUMN_MIDDLE},
	/*-----------------------------------------------------------------------*/
	/*------------------------------ПРАВАЯ РУКА------------------------------*/
	{6, 0, 1, 1, 0,  HAND_RIGHT, FINGER_THUMB,   ROW_MIDDLE, COLUMN_MIDDLE},
	{7, 0, 1, 1, 0,  HAND_RIGHT, FINGER_INDEX,   ROW_MIDDLE, COLUMN_MIDDLE},
	{8, 0, 1, 1, 0,  HAND_RIGHT, FINGER_MIDDLE,  ROW_MIDDLE, COLUMN_MIDDLE},
	{9, 0, 1, 1, 0,  HAND_RIGHT, FINGER_ANNULAR, ROW_MIDDLE, COLUMN_MIDDLE},
	{10, 0, 1, 1, 0, HAND_RIGHT, FINGER_PINKY,   ROW_MIDDLE, COLUMN_MIDDLE},
	/*-----------------------------------------------------------------------*/
};

std::vector<Layout::LayoutSymbols> testLayoutKeys = {
	/*-----------------------------------------------------------------------*/
	/*-----------ПЕРВЫЙ СЛОЙ-----------*/ /*-----------ВТОРОЙ СЛОЙ-----------*/
	{{0, 0}, L"a"},   /* LEFT  PINKY   */ {{1, 0}, L"②"},   /* LEFT  PINKY   */
	{{0, 1}, L"b"},   /* LEFT  ANNULAR */ {{1, 1}, L"A"},   /* LEFT  ANNULAR */
	{{0, 2}, L"c"},   /* LEFT  MIDDLE  */ {{1, 2}, L"B"},   /* LEFT  MIDDLE  */
	{{0, 3}, L"d"},   /* LEFT  INDEX   */ {{1, 3}, L"C"},   /* LEFT  INDEX   */
	{{0, 4}, L" "},   /* LEFT  THUMB   */ {{1, 4}, L"③"},   /* LEFT  THUMB   */
	{{0, 5}, L"①"},   /* RIGHT THUMB   */ {{1, 5}, L" "},   /* RIGHT THUMB   */
	{{0, 6}, L", "},  /* RIGHT INDEX   */ {{1, 6}, L","},   /* RIGHT INDEX   */
	{{0, 7}, L". ①"}, /* RIGHT MIDDLE  */ {{1, 7}, L"."},   /* RIGHT MIDDLE  */
	{{0, 8}, L"the"}, /* RIGHT ANNULAR */ {{1, 8}, L"The"}, /* RIGHT ANNULAR */
	{{0, 9}, L"②"},   /* RIGHT PINKY   */ {{1, 9}, L"D"},   /* RIGHT PINKY   */
	/*-----------------------------------------------------------------------*/
	/*-----------ТРЕТИЙ СЛОЙ-----------*/ /*---------ЧЕТВЕРТЫЙ СЛОЙ----------*/
	{{2, 0}, L"e"},   /* LEFT  PINKY   */ {{3, 0}, L"E"},   /* LEFT  PINKY   */
	{{2, 1}, L"f"},   /* LEFT  ANNULAR */ {{3, 1}, L"Й"},   /* LEFT  ANNULAR */
	{{2, 2}, L"g"},   /* LEFT  MIDDLE  */ {{3, 2}, L"G"},   /* LEFT  MIDDLE  */
	{{2, 3}, L"h"},   /* LEFT  INDEX   */ {{3, 3}, L"H"},   /* LEFT  INDEX   */
	{{2, 4}, L"i"},   /* LEFT  THUMB   */ {{3, 4}, L"I"},   /* LEFT  THUMB   */
	{{2, 5}, L"③"},   /* RIGHT THUMB   */ {{3, 5}, L"⓪"},   /* RIGHT THUMB   */
	{{2, 6}, L" - "}, /* RIGHT INDEX   */ {{3, 6}, L"-"},   /* RIGHT INDEX   */
	{{2, 7}, L"; "},  /* RIGHT MIDDLE  */ {{3, 7}, L";"},   /* RIGHT MIDDLE  */
	{{2, 8}, L"THE"}, /* RIGHT ANNULAR */ {{3, 8}, L"the"}, /* RIGHT ANNULAR */
	{{2, 9}, L"{}"},  /* RIGHT PINKY   */ {{3, 9}, L"[]"},  /* RIGHT PINKY   */
	/*-----------------------------------------------------------------------*/
};

//-----------------------------------------------------------------------------
TEST_CASE("getSymbols, getKeys, getLayerKeys") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout test(tenkey, testLayoutKeys);

	// getSymbols
	CHECK(test.getSymbols({0, 0}) == L"a");
	CHECK(test.getSymbols({1, 1}) == L"A");
	CHECK(test.getSymbols({2, 0}) == L"e");
	CHECK(test.getSymbols({3, 8}) == L"the");
	CHECK(test.getSymbols({3, 5}) == L"⓪");
	CHECK(test.getSymbols({3, 1}) == L"Й");

	// getKeys
	CHECK(test.getKeys(L't').size() == 2);
	CHECK(test.getSymbols(test.getKeys(L't')[0]) == L"the");
	CHECK(test.getSymbols(test.getKeys(L't')[1]) == L"the");
	CHECK(test.getKeys(L'I').size() == 1);
	CHECK(test.getKeys(L',').size() == 2);
	CHECK(test.getKeys(L' ').size() == 3);

	// getLayerKeys
	CHECK(test.getLayerKeys(0, 1) == KeyPoses({5}));
	CHECK(test.getLayerKeys(0, 2) == KeyPoses({9}));
	CHECK(test.getLayerKeys(0, 3) == KeyPoses({5, 4}));
	CHECK(test.getLayerKeys(1, 2) == KeyPoses({0}));
	CHECK(test.getLayerKeys(1, 3) == KeyPoses({4}));
	CHECK(test.getLayerKeys(2, 3) == KeyPoses({5}));

	// getLayerKeys invert order
	CHECK(test.getLayerKeys(1, 0) == KeyPoses({4, 5}));
	CHECK(test.getLayerKeys(2, 0) == KeyPoses({5, 5}));
	CHECK(test.getLayerKeys(3, 0) == KeyPoses({5}));
	CHECK(test.getLayerKeys(2, 1) == KeyPoses({5, 5, 5}));
	CHECK(test.getLayerKeys(3, 1) == KeyPoses({5, 5}));
	CHECK(test.getLayerKeys(3, 2) == KeyPoses({5, 9}));
}

//-----------------------------------------------------------------------------
TEST_CASE("typeKeys") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout test(tenkey, testLayoutKeys);

	CHECK(test.typeKeys({{0, 0}, {0, 7}, {1, 2}, {2, 9}}) == L"a. B{}");
	CHECK(test.typeKeys({{1, 8}, {0, 6}, {1, 9}, {2, 0}}) == L"The, De");
	CHECK(test.typeKeys({{3, 2}, {3, 3}, {3, 7}, {1, 5}}) == L"GH; ");
	CHECK(test.typeKeys({{2, 8}, {1, 8}, {0, 8}, {2, 6}}) == L"THEThethe - ");
	CHECK(test.typeKeys({{0, 0}, {0, 0}, {0, 4}, {0, 7}}) == L"aa . ");
}

//-----------------------------------------------------------------------------
TEST_CASE("typeTaps") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout test(tenkey, testLayoutKeys);
	Taps taps;
	PhysicalState state(0);

	state = PhysicalState(0);
	taps = {
		{0, PRESS_ONCE}, {7, PRESS_ONCE}, {2, PRESS_ONCE}, {5, PRESS_DOWN},
		{0, PRESS_DOWN}, {9, PRESS_ONCE}, {5, PRESS_UP}, {0, PRESS_UP}
	};
	CHECK(test.typeTaps(taps, state) == L"a. B{}");

	state = PhysicalState(1);
	taps = {
		{8, PRESS_ONCE}, {4, PRESS_ONCE}, {5, PRESS_ONCE}, {6, PRESS_ONCE},
		{9, PRESS_ONCE}, {0, PRESS_ONCE}, {0, PRESS_ONCE}
	};
	CHECK(test.typeTaps(taps, state) == L"The, De");

	state = PhysicalState(1);
	taps = {
		{4, PRESS_DOWN}, {2, PRESS_ONCE}, {3, PRESS_ONCE}, {7, PRESS_ONCE},
		{4, PRESS_UP}, {5, PRESS_ONCE}
	};
	CHECK(test.typeTaps(taps, state) == L"GH; ");

	state = PhysicalState(2);
	taps = {
		{8, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},
		{8, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},{8, PRESS_ONCE},
		{6, PRESS_ONCE}
	};
	CHECK(test.typeTaps(taps, state) == L"THEThethe - ");

	state = PhysicalState(0);
	taps = {
		{0, PRESS_ONCE},{0, PRESS_ONCE},{7, PRESS_ONCE},{6, PRESS_ONCE},
		{1, PRESS_ONCE}
	};
	CHECK(test.typeTaps(taps, state) == L"aa. ,b");
}