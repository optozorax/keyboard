#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include <kbd/keyboard.h>
#include <kbd/combinatorics.h>

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
std::pair<int, int> Keyboard::getPos(KeyPos key) const {
	return {m_keys[key].x, m_keys[key].y};
}

//-----------------------------------------------------------------------------
std::pair<int, int> Keyboard::getSize(KeyPos key) const {
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
std::vector<Keyboard::KeyboardKey> Keyboard::getKeyboardInnerFormat(void) const {
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
			   const std::vector<LayoutSymbols>& symbols) : Keyboard(keyboard), m_symbols(symbols) {
	using namespace boost;

	// Подсчет числа слоёв
	int layers = 0;
	for (const auto& i : symbols)
		if (i.key.layer > layers)
			layers = i.key.layer;

	// Инициализируем массив заданным числом слоёв
	m_layerMas = std::vector<std::vector<std::wstring>>(layers+1, std::vector<std::wstring>(keyboard.size()));

	// Заполняем массив слоёв клавишами
	for (const auto& i : symbols)
		m_layerMas[i.key.layer][i.key.key] = i.symbols;
 
	// Инициализируем map
	for (const auto& i : symbols)
		m_keyMap[i.symbols[0]].push_back(i.key);

	// Инициализируем map для слоёв
	typedef adjacency_list<
		listS, vecS, directedS, 
		no_property, property<edge_weight_t, int>> graph_t;
	typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
	typedef graph_traits<graph_t>::vertex_iterator vertex_iterator;
	typedef std::pair<int, int> Edge;

	// Сначала составляем масссив смежности для всех ребер
	int num_layers = 20;
	std::vector<Edge> edge_array;
	std::map<Edge, KeyPos> edge_map;
	for (const auto& i : symbols) {
		auto layer = getLayer(i.symbols[0]);
		if (i.symbols.size() == 1 && layer) {
			edge_array.push_back({i.key.layer, *layer});
			edge_map[edge_array.back()] = i.key.key;
		}
	}

	// Создаем граф и прочую непонятную вещь, которую я нагло украл из примера boost
	std::vector<int> weights(edge_array.size(), 1);
	graph_t g(edge_array.begin(), edge_array.end(), weights.begin(), num_layers);
	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));

	// Перебираем все слои
	for (int i = 0; i < num_layers; i++) {
		// Ставим за точку отсчета текущий слой
		vertex_descriptor s = vertex(i, g);

		// Находим все пути от текущего слоя до остальных
		dijkstra_shortest_paths(g, s,
			predecessor_map(make_iterator_property_map(
				p.begin(), 
				get(vertex_index, g))
			).
			distance_map(make_iterator_property_map(
				d.begin(), 
				get(vertex_index, g)
			))
		);

		// Перебираем все вершины, для каждой вершины находим путь от текущего слоя i, преобразуем его в KeyPos, закидываем в map
		vertex_iterator vi, vend;
		for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
			std::vector<int> path;
			KeyPoses keys;
			path.push_back(*vi);
			auto current = p[*vi];
			while (current != i) {
				path.push_back(current);
				current = p[current];

				// Костыль для тех слоёв, которые с текущим не находятся в одной смежности
				if (p[current] == current && current != i)
					goto next;
			}
			path.push_back(current);
			std::reverse(path.begin(), path.end());

			// Преобразуем направление в KeyPos
			for (int k = 0; k < path.size() - 1; ++k) {
				try {
					keys.push_back(edge_map.at({ path[k], path[k + 1] }));
				} catch (...) {
					goto next;
				}
			}

			// Закидываем это в map
			m_layerMap[{i, *vi}] = keys;
			next:;
		}
	}
}

//-----------------------------------------------------------------------------
const std::wstring& Layout::getSymbols(Key key) const {
	return m_layerMas[key.layer][key.key];
}

//-----------------------------------------------------------------------------
const Keys& Layout::getKeys(wchar_t letter) const {
	return m_keyMap.at(letter);
}

//-----------------------------------------------------------------------------
const KeyPoses&	Layout::getLayerKeys(int currentLayer, int toLayer) const {
	try {
		return m_layerMap.at({currentLayer, toLayer});
	} catch (...) {
		return {};
	}
}

//-----------------------------------------------------------------------------
const std::vector<Layout::LayoutSymbols>& Layout::getLayoutInnerFormat(void) const {
	return m_symbols;
}

//-----------------------------------------------------------------------------
std::wstring Layout::typeTaps(const Taps& taps, PhysicalState& state) const {
	return {};
}

//-----------------------------------------------------------------------------
std::wstring Layout::typeKeys(const Keys& keys) const {
	return {};
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

Typer::Typer(const Layout& layout) : m_layout(layout) {
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
std::vector<Keys> decomposeToKeys(const Layout& layout, const std::wstring& text, int maxOneHandSize, int& symbolsCount) {
	std::vector<int> variants(text.size(), 0);
	for (int i = 0; i < text.size(); ++i)
		variants[i] = layout.getKeys(text[i]).size();

	std::vector<Keys> allVariants;

	// Находим максимальную длину первой однорукой части
	{
		symbolsCount = 0;
		Number num(variants);
		do {
			auto res = num.get();

			auto lastHand = layout.getHand(layout.getKeys(text[0])[res[0]].key);
			int i = 1;
			while (lastHand == layout.getHand(layout.getKeys(text[i])[res[i]].key))
				i++;

			if (i > symbolsCount)
				symbolsCount = i;

			num++;
		} while (!num.isEnd());

		// Модифицируем данные, чтобы они соответствовали выбранной длине
		if (symbolsCount > maxOneHandSize)
			symbolsCount = maxOneHandSize;

		variants = std::vector<int>(variants.begin(), variants.begin() + symbolsCount);
	}

	// Перебираем все варианты
	Number num(variants);
	do {
		auto res = num.get();

		// Формируем какие строки будут после каждого нажатия
		std::vector<std::wstring> strings(text.size());
		std::vector<Key> keys(text.size());
		for (int i = 0; i < text.size(); ++i) {
			keys[i] = layout.getKeys(text[i])[res[i]];
			strings[i] = layout.getSymbols(keys[i]);
		}

		// Убираем лишние части, если есть строки с несколькими символами, если они подходят, если не подходят, то этот вариант проигрышный. Так же здесь фильтруем автоматическое переключение слоя
		int pos = 0;
		for (int i = 0; i < strings.size(); ++i) {
			if (strings[i].size() != 1) {
				// Проверяем, чтобы дальнейший текст соответствовал тому, что автоматически наберется в клавише с несколькими символами
				for (int j = 0; j < strings[i].size(); ++j) {
					if (!getLayer(strings[i][j]))
						if (pos + j < text.size()) {
							if (text[pos + j] != strings[i][j])
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

	return allVariants;
}

//-----------------------------------------------------------------------------
Taps decomposeToTaps(const Layout& layout, const Keys& keys, PhysicalState& state) {
	return {};
}

//-----------------------------------------------------------------------------
Accords decomposeToAccords(const KeyPoses& keyPoses) {
	return {};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double evalTime(const Keyboard& keyboard, const Accords& phrase) {
	return {};
}

//-----------------------------------------------------------------------------
Accords parseText(const Layout& layout, const std::wstring& text) {
	return {};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
TyperAlternationLover::TyperAlternationLover(const Layout& layout) : Typer(layout) {
}

//-----------------------------------------------------------------------------
int TyperAlternationLover::getOptimalAccords(const std::vector<Accords>& variants) const {
	return {};
}

//-----------------------------------------------------------------------------
double TyperAlternationLover::type(const Accords& accords) {
	return {};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
TyperAccordsLover::TyperAccordsLover(const Layout& layout) : Typer(layout) {
}

//-----------------------------------------------------------------------------
int TyperAccordsLover::getOptimalAccords(const std::vector<Accords>& variants) const {
	return {};
}

//-----------------------------------------------------------------------------
double TyperAccordsLover::type(const Accords& accords) {
	return {};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RealTyper::RealTyper(const Layout& layout) : Typer(layout) {
}

//-----------------------------------------------------------------------------
int RealTyper::getOptimalAccords(const std::vector<Accords>& variants) const {
	return {};
}

//-----------------------------------------------------------------------------
double RealTyper::type(const Accords& accords) {
	return {};
}

//-----------------------------------------------------------------------------
std::optional<int> getLayer(wchar_t symbol) {
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