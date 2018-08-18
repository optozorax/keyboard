#pragma once

#include <kbd/keyboard.h>

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

std::vector<Layout::LayoutSymbols> tenkeyLayout1 = {
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