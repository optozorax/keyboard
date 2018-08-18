#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <kbd/keyboard.h>
#include "keyboards.h"

using namespace kbd;

//-----------------------------------------------------------------------------
TEST_CASE("getSymbols, getKeys, getLayerKeys") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout layout(tenkey, tenkeyLayout1);

	// getSymbols
	CHECK(layout.getSymbols({0, 0}) == L"a");
	CHECK(layout.getSymbols({1, 1}) == L"A");
	CHECK(layout.getSymbols({2, 0}) == L"e");
	CHECK(layout.getSymbols({3, 8}) == L"the");
	CHECK(layout.getSymbols({3, 5}) == L"⓪");
	CHECK(layout.getSymbols({3, 1}) == L"Й");

	// getKeys
	CHECK(layout.getKeys(L't').size() == 2);
	CHECK(layout.getSymbols(layout.getKeys(L't')[0]) == L"the");
	CHECK(layout.getSymbols(layout.getKeys(L't')[1]) == L"the");
	CHECK(layout.getKeys(L'I').size() == 1);
	CHECK(layout.getKeys(L',').size() == 2);
	CHECK(layout.getKeys(L' ').size() == 3);

	auto isKeyPosesEqual = [] (const KeyPoses& a, const KeyPoses& b) -> bool {
		if (a.size() != b.size())
			return false;
		for (int i = 0; i < a.size(); i++)
			if (a[i] != b[i])
				return false;
		return true;
	};
	auto compareKeyPosesArrays = [&isKeyPosesEqual] (std::vector<KeyPoses> a, std::vector<KeyPoses> b) {
		if (a.size() != b.size()) {
			CHECK(a.size() == b.size());
			return;
		}
		for (int i = 0; i < a.size(); i++) {
			for (int j = 0; j < b.size(); j++) {
				if (isKeyPosesEqual(a[i], b[j])) {
					a.erase(a.begin() + i);
					b.erase(b.begin() + j);
					goto next;
				}
			}
			CHECK(a[i] == b[0]);
			next:;
		}
	};

	// getLayerKeys
	compareKeyPosesArrays(layout.getLayerKeys(0, 1), {{5}});
	compareKeyPosesArrays(layout.getLayerKeys(0, 2), {{9}, {5, 0}});
	compareKeyPosesArrays(layout.getLayerKeys(0, 3), {{5, 4}, {9, 5}, {5, 0, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(1, 2), {{0}, {4, 5, 9}});
	compareKeyPosesArrays(layout.getLayerKeys(1, 3), {{4}, {0, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(2, 3), {{5}});

	// getLayerKeys invert order
	compareKeyPosesArrays(layout.getLayerKeys(1, 0), {{4, 5}, {0, 5, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(2, 0), {{5, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(3, 0), {{5}});
	compareKeyPosesArrays(layout.getLayerKeys(2, 1), {{5, 5, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(3, 1), {{5, 5}});
	compareKeyPosesArrays(layout.getLayerKeys(3, 2), {{5, 9}, {5, 5, 0}});
}

//-----------------------------------------------------------------------------
TEST_CASE("typeKeys") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout layout(tenkey, tenkeyLayout1);

	CHECK(layout.typeKeys({{0, 0}, {0, 7}, {1, 2}, {2, 9}}) == L"a. B{}");
	CHECK(layout.typeKeys({{1, 8}, {0, 6}, {1, 9}, {2, 0}}) == L"The, De");
	CHECK(layout.typeKeys({{3, 2}, {3, 3}, {3, 7}, {1, 5}}) == L"GH; ");
	CHECK(layout.typeKeys({{2, 8}, {1, 8}, {0, 8}, {2, 6}}) == L"THEThethe - ");
	CHECK(layout.typeKeys({{0, 0}, {0, 0}, {0, 4}, {0, 7}}) == L"aa . ");
}

//-----------------------------------------------------------------------------
TEST_CASE("typeTaps") {
	Keyboard tenkey("tenkey", tenkeyKeys);
	Layout layout(tenkey, tenkeyLayout1);
	Taps taps;
	PhysicalState state(0);

	state = PhysicalState(0);
	taps = {
		{0, PRESS_ONCE}, {7, PRESS_ONCE}, {2, PRESS_ONCE}, {5, PRESS_DOWN},
		{0, PRESS_DOWN}, {9, PRESS_ONCE}, {5, PRESS_UP}, {0, PRESS_UP}
	};
	CHECK(layout.typeTaps(taps, state) == L"a. B{}");

	state = PhysicalState(1);
	taps = {
		{8, PRESS_ONCE}, {4, PRESS_ONCE}, {5, PRESS_ONCE}, {6, PRESS_ONCE},
		{9, PRESS_ONCE}, {0, PRESS_ONCE}, {0, PRESS_ONCE}
	};
	CHECK(layout.typeTaps(taps, state) == L"The, De");

	state = PhysicalState(1);
	taps = {
		{4, PRESS_DOWN}, {2, PRESS_ONCE}, {3, PRESS_ONCE}, {7, PRESS_ONCE},
		{4, PRESS_UP}, {5, PRESS_ONCE}
	};
	CHECK(layout.typeTaps(taps, state) == L"GH; ");

	state = PhysicalState(2);
	taps = {
		{8, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},
		{8, PRESS_ONCE},{5, PRESS_ONCE},{5, PRESS_ONCE},{8, PRESS_ONCE},
		{6, PRESS_ONCE}
	};
	CHECK(layout.typeTaps(taps, state) == L"THEThethe - ");

	state = PhysicalState(0);
	taps = {
		{0, PRESS_ONCE},{0, PRESS_ONCE},{7, PRESS_ONCE},{6, PRESS_ONCE},
		{1, PRESS_ONCE}
	};
	CHECK(layout.typeTaps(taps, state) == L"aa. ,b");
}