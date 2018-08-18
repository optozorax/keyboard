#include <vector>
#include <algorithm>
#include <set>

#include <kbd/keyboard.h>
#include <kbd/combinatorics.h>

namespace kbd
{

//-----------------------------------------------------------------------------
// Ребро ориентированного графа
struct Edge {
	int a;
	int b;

	KeyPos key; // Клавиша, которая с слоя a включает слой b
};

typedef std::vector<Edge> Edges;

//-----------------------------------------------------------------------------
// Класс ориентированного графа
class DirectedGraph
{
public:
	void addEdge(Edge ed);
	int getEdgeCount(void) const;
	Edges getAdjacent(int i) const;
	Edge getEdge(int i) const;
	void deleteEdge(int i);
	std::vector<int> getVertixes(void) const;
private:
	std::vector<Edge> m_edges;
};

//-----------------------------------------------------------------------------
// Обходит граф, при этом инициализирует map, который отвечает за все возможные методы перехода с одного слоя на другой слой
void travelGraph_InitMap(
	const DirectedGraph& graph,
	std::map<std::pair<int, int>, std::vector<KeyPoses>>& layerMap,
	int firstLayer);

// Рекурсивная часть верхней функции
void travelGraph_InitMap_r(
	const DirectedGraph& graph,
	std::map<std::pair<int, int>, std::vector<KeyPoses>>& layerMap,
	KeyPoses path,
	std::set<int> attendedLayers,
	int firstLayer,
	int currentLayer,
	int currentKey
);

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
void DirectedGraph::addEdge(Edge ed) {
	m_edges.push_back(ed);
}

//-----------------------------------------------------------------------------
int DirectedGraph::getEdgeCount(void) const {
	return m_edges.size();
}

//-----------------------------------------------------------------------------
Edges DirectedGraph::getAdjacent(int k) const {
	Edges result;
	for (const auto& ed : m_edges) {
		if (ed.a == k)
			result.push_back(ed);
	}

	return result;
}

//-----------------------------------------------------------------------------
Edge DirectedGraph::getEdge(int i) const {
	return m_edges[i];
}

//-----------------------------------------------------------------------------
void DirectedGraph::deleteEdge(int i) {
	m_edges.erase(m_edges.begin() + i);
}

//-----------------------------------------------------------------------------
std::vector<int> DirectedGraph::getVertixes(void) const {
	std::vector<int> result;
	for (const auto& ed : m_edges) {
		result.push_back(ed.a);
		result.push_back(ed.b);
	}
	std::sort(result.begin(), result.end());
	result.erase(std::unique(result.begin(), result.end()), result.end());
	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void travelGraph_InitMap(
	const DirectedGraph& graph,
	std::map<std::pair<int, int>, std::vector<KeyPoses>>& layerMap,
	int firstLayer) {
	travelGraph_InitMap_r(graph, layerMap, {}, {}, firstLayer, firstLayer, 0);
}

//-----------------------------------------------------------------------------
void travelGraph_InitMap_r(
	const DirectedGraph& graph,
	std::map<std::pair<int, int>, std::vector<KeyPoses>>& layerMap,
	KeyPoses path,
	std::set<int> attendedLayers,
	int firstLayer,
	int currentLayer,
	int currentKey
) {
	attendedLayers.insert(currentLayer);
	if (currentLayer != firstLayer)
		path.push_back(currentKey);
	if (path.size() != 0) {
		// Ищем дубликаты текущего пути. Если их нет, то добавляем
		auto& paths = layerMap[{firstLayer, currentLayer}];

		bool isAdd = true;
		for (const auto& i : paths)
			if (i.size() == path.size()) {
				bool isEqual = true;
				for (int j = 0; j < i.size(); ++j)
					isEqual &= i[j] == path[j];
				if (isEqual) {
					isAdd = false;
					break;
				}
			}
		if (isAdd)
			paths.push_back(path);
	}

	auto adj = graph.getAdjacent(currentLayer);
	for (auto& i : adj) {
		if (attendedLayers.find(i.b) == attendedLayers.end())
			travelGraph_InitMap_r(graph, layerMap, path, attendedLayers, firstLayer, i.b, i.key);
	}
}

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
PhysicalState::PhysicalState(int defaultLayer) : isOneTap(false), oneTapLayer(0) {
	layerStack.push_back({-1, defaultLayer});
}

//-----------------------------------------------------------------------------
void PhysicalState::addOneTap(int layer) {
	isOneTap = true;
	oneTapLayer = layer;
}

//-----------------------------------------------------------------------------
int PhysicalState::getCurrentLayer(void) const {
	if (isOneTap) {
		isOneTap = false;
		return oneTapLayer;
	} else
		return layerStack.back().second;
}

//-----------------------------------------------------------------------------
std::optional<KeyPos> PhysicalState::isFingerBusy(Hand hand, Finger finger) const {
	try {
		return busyFingers.at({ hand, finger });
	} catch (...) {
		return std::nullopt;
	}
}

//-----------------------------------------------------------------------------
void PhysicalState::busyFinger(Hand hand, Finger finger, KeyPos key, int layer) {
	if (hand == HAND_ANY || finger == FINGER_ANY)
		throw std::exception();
	busyFingers[{hand, finger}] = key;
	FingerId fingerId = (hand-1)*5 + finger-1;
	layerStack.push_back({fingerId, layer});
}

//-----------------------------------------------------------------------------
bool PhysicalState::unbusyFinger(Hand hand, Finger finger) {
	FingerId fingerId = (hand-1)*5 + finger-1;
	if (busyFingers[{hand, finger}] == std::nullopt)
		throw std::exception();
	busyFingers[{hand, finger}] = std::nullopt;
	for (int j = 0; j < layerStack.size(); j++) {
		if (layerStack[j].first == fingerId) {
			layerStack.erase(layerStack.begin() + j);
			break;
		}
	}
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
	//-------------------------------------------------------------------------
	// Заполняем массив слоёв с клавишами

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
 
 	//-------------------------------------------------------------------------
	// Инициализируем map для клавиш
	for (const auto& i : symbols)
		m_keyMap[i.symbols[0]].push_back(i.key);

	//-------------------------------------------------------------------------
	// Инициализируем map для слоёв

	// Создаём граф по слоям
	DirectedGraph graph;
	for (const auto& i : symbols) {
		auto layer = getLayer(i.symbols[0]);
		if (i.symbols.size() == 1 && layer)
			graph.addEdge({i.key.layer, *layer, i.key.key});
	}

	// Перебираем все вершины и из них находим все пути до других вершин
	auto vertixes = graph.getVertixes();
	for (const auto& i : vertixes)
		travelGraph_InitMap(graph, m_layerMap, i);
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
const std::vector<KeyPoses>& Layout::getLayerKeys(int currentLayer, int toLayer) const {
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
	std::wstring result;
	for (auto& i : taps) {
		auto symbols = getSymbols({ state.getCurrentLayer(), i.key });
		auto isCurrentFingerBusy = state.isFingerBusy(getHand(i.key), getFinger(i.key));
		auto isLayerKey = getLayer(symbols[0]);
		if (i.press != PRESS_ONCE) {
			if (i.press == PRESS_DOWN) {
				if (isCurrentFingerBusy || !isLayerKey) {
					throw std::exception("You tried to busy busied finger or to busy not layer key.");
				} else {
					state.busyFinger(getHand(i.key), getFinger(i.key), i.key, *isLayerKey);
				}
			} else {
				if (!isCurrentFingerBusy || *isCurrentFingerBusy != i.key) {
					throw std::exception("You tried to unbusied wrong finger.");
				} else {
					state.unbusyFinger(getHand(i.key), getFinger(i.key));
				}
			}
		} else {
			if (isCurrentFingerBusy) {
				throw std::exception("You tried to press key by busied finger.");
			} else {
				auto layer = getLayer(symbols.back());
				if (layer) {
					state.addOneTap(*layer);
					symbols.pop_back();
				}
				result += symbols;
			}
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
std::wstring Layout::typeKeys(const Keys& keys) const {
	std::wstring result;
	for (const auto& i : keys) {
		auto add = getSymbols(i);
		if (getLayer(add.back()))
			add.pop_back();
		result += add;
	}
	return result;
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
std::vector<Taps> decomposeToTaps(const Layout& layout, const Keys& keys, PhysicalState& state) {
	/** 
	Критерий, чтобы сделать зажатие клавиши для включения некоторого слоя:
	Более 1 буквы набирается в другом и одном и том же слое. 
	1 буква и одна клавиша для включения другого слоя набираются в другом слое.
	Когда отключать слои?
	Емоё, как всё сложно.
	Мне действительно это нужно? :)
	 */
	return {};
}

//-----------------------------------------------------------------------------
std::vector<Accords> decomposeOneHandAccords(const Keyboard& keyboard, const KeyPoses& keyPoses) {
	std::vector<Accords> result;
	Compositions comp(keyPoses.size());
	int pos;
	Accords accords;
	do {
		// Сначала проверяется чтобы каждый аккорд был на разных пальцах
		auto res = comp.get();
		pos = 0;
		for (const auto& i : res) {
			for (int j = 1; j < i; ++j) {
				if (keyboard.getFinger(keyPoses[pos]) == keyboard.getFinger(keyPoses[pos + j]))
					goto not_push;
			}
			pos += i;
		}

		// Далее проверяется, чтобы не было аккордов из одной буквы, если они не на одном пальце
		pos = 0;
		for (int i = 0; i < res.size() - 1; ++i) {
			if (res[i] == 1 && res[i + 1] == 1 &&
				keyboard.getFinger(keyPoses[pos]) != keyboard.getFinger(keyPoses[pos + 1]))
				goto not_push;

			pos += res[i];
		}

		// Если все проверки пройдены, то только тогда этот варивант можно положить
		pos = 0;
		accords.clear();
		for (int i = 0; i < res.size(); ++i) {
			accords.push_back({});
			for (int j = 0; j < i; ++j) {
				accords.back().push_back(keyPoses[pos]);
				pos++;
			}
		}
		result.push_back(accords);
		not_push:;

		comp++;
	} while (comp.isEnd());

	return result;
}

//-----------------------------------------------------------------------------
std::vector<Accords> decomposeToAccords(const Keyboard& keyboard, const KeyPoses& keyPoses) {
	// Формируем части каждой руки
	std::vector<std::pair<KeyPoses, Hand>> parts;
	for (const auto& i : keyPoses) {
		if (parts.size() == 0)
			parts.push_back({{i}, keyboard.getHand(i) });
		else {
			if (parts.back().second == keyboard.getHand(i)) {
				parts.back().first.push_back(i);
			} else {
				parts.push_back({{i}, keyboard.getHand(i)});
			}
		}
	}

	// Получаем все варианты набора каждой рукой
	std::vector<std::vector<Accords>> allVariants;
	std::vector<int> variants;
	for (const auto& i : parts) {
		allVariants.push_back(decomposeOneHandAccords(keyboard, i.first));
		variants.push_back(allVariants.back().size());
	}

	// Перебираем все эти варианты и помещаем в результат
	std::vector<Accords> result;
	Number num(variants);
	do {
		auto res = num.get();
		result.push_back({});
		for (int i = 0; i < res.size(); ++i) {
			result.back().insert(result.back().end(), allVariants[i][res[i]].begin(), allVariants[i][res[i]].begin());
		}
		num++;
	} while (!num.isEnd());

	return result;
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