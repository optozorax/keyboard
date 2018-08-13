#include <keyboard.h>

namespace kbd
{

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Keyboard::Keyboard() {
}

//-----------------------------------------------------------------------------
Keyboard::Keyboard(std::string name, 
				   const std::vector<KeyboardKey>& keys) : m_name(name), m_keys(keys) {
}	

//-----------------------------------------------------------------------------
int Keyboard::size(void) const {
	return m_keys.size();
}

//-----------------------------------------------------------------------------
std::string Keyboard::getName(void) const {
	return m_name;
}

//-----------------------------------------------------------------------------
Hand Keyboard::getHand(KeyPos key) const {
	return m_keys[key].hand;
}

//-----------------------------------------------------------------------------
Finger Keyboard::getFinger(KeyPos key) const {
	return m_keys[key].finger;
}

//-----------------------------------------------------------------------------
Row Keyboard::getRow(KeyPos key) const {
	return m_keys[key].row;
}

//-----------------------------------------------------------------------------
Column Keyboard::getColumn(KeyPos key) const {
	return m_keys[key].column;
}

//-----------------------------------------------------------------------------
pair<int, int> Keyboard::getPos(KeyPos key) const {
	return {m_keys[key].x, m_keys[key].y};
}

//-----------------------------------------------------------------------------
pair<int, int> Keyboard::getSize(KeyPos key) const {
	return {m_keys[key].xsize, m_keys[key].ysize};
}

//-----------------------------------------------------------------------------
double Keyboard::getAngle(KeyPos key) const {
	return m_keys[key].angle;
}

//-----------------------------------------------------------------------------
std::vector<KeyPos> Keyboard::getKeys(Hand hand,
									  Finger finger,
									  Row row,
									  Column column) const {
	std::vector<KeyPos> result;
	for (int i = 0; i < m_keys.size(); ++i) {
		auto& key = m_keys[i];

		bool isHand = (hand == HAND_ANY) || (hand == key.hand);
		bool isFinger = (finger == FINGER_ANY) || (finger == key.finger);
		bool isRow = (row == ROW_ANY) || (row == key.row);
		bool isColumn = (column == COLUMN_ANY) || (column == key.column);

		if (isHand && isFinger && isRow && isColumn)
			result.push_back(i);
	}

	// Если задано всё, то число клавиш обязано быть 0 или 1
	if (hand != HAND_ANY && 
		finger != FINGER_ANY && 
		row != ROW_ANY && 
		column != COLUMN_ANY && 
		result.size() > 1)
		throw std::exception();
}

//-----------------------------------------------------------------------------
std::vector<KeyboardKey> Keyboard::getKeyboardInnerFormat(void) const {
	return m_keys;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Layout::Layout() {
}

//-----------------------------------------------------------------------------
Layout::Layout(const Keyboard& keyboard,
			   const std::vector<LayoutSymbols>& symbols) : Keybard(keyboard), m_symbols(symbols) {
	// TODO добавить обработку полученных данных в map, чтобы ускорить процесс поиска
}

//-----------------------------------------------------------------------------
Symbols Layout::getSymbols(Key key) const {

}

//-----------------------------------------------------------------------------
std::vector<Key> Layout::getKeys(wchar_t letter) const {

}

//-----------------------------------------------------------------------------
std::vector<LayoutSymbols> Layout::getLayoutInnerFormat(void) const {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void saveToFile(const Keyboard& keyboard, std::string keyboardFile) {

}

//-----------------------------------------------------------------------------
void readFromFile(Keyboard& keyboard, std::string keyboardFile) {

}

//-----------------------------------------------------------------------------
void saveToFile(const Layout& layout, std::string layoutFile) {

}

//-----------------------------------------------------------------------------
void readFromFile(Layout& layout, std::string layoutFile, std::string keyboardFile) {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
std::optional<std::vector<Accords>> decomposeToAccords(const Layout& layout, std::wstring str, int maxOneHandSize) {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double evalTime(const Keyboard& keyboard, const Accords& phrase) {

}

//-----------------------------------------------------------------------------
Accords parseText(const Layout& layout, const std::wstring& text) {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
std::optional<int> getLayer(Symbol symbol) {
	switch (symbol)	 {
		case L'⓪':
			return 0;
		case L'①':
			return 1;
		case L'②':
			return 2;
		case L'③':
			return 3;
		case L'④':
			return 4;
		case L'⑤':
			return 5;
		case L'⑥':
			return 6;
		case L'⑦':
			return 7;
		case L'⑧':
			return 8;
		case L'⑨':
			return 9;
		case L'⑩':
			return 10;
		case L'⑪':
			return 11;
		case L'⑫':
			return 12;
		case L'⑬':
			return 13;
		case L'⑭':
			return 14;
		case L'⑮':
			return 15;
		case L'⑯':
			return 16;
		case L'⑰':
			return 17;
		case L'⑱':
			return 18;
		case L'⑲':
			return 19;
		case L'⑳':
			return 20;
	};

	return std::nullopt;
}

};