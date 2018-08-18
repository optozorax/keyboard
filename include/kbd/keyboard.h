#pragma once

#include <vector>
#include <map>
#include <string>
#include <optional>

namespace kbd
{

	//-------------------------------------------------------------------------
	enum Press
	{
		PRESS_ONCE, // Клавиша нажимается единожды
		PRESS_DOWN, // Клавиша зажимается, это может быть только клавиша для переключения слоя, причем палец, который зажимает эту клавишу становится занятым, и не может более использоваться для 
		PRESS_UP // Клавиша отпускается, аналогично, только для переключения слоя
	};

	enum Hand
	{
		HAND_ANY, // Обозначет любую руку
		HAND_LEFT,
		HAND_RIGHT
	};

	enum Finger
	{
		FINGER_ANY, // Обозначет любой палец
		FINGER_PINKY,
		FINGER_ANNULAR,
		FINGER_MIDDLE,
		FINGER_INDEX,
		FINGER_THUMB // Касательно большого пальца: у него движения вправо-влево является сменой ряда, а не колонки. Колонка сменится, если большой палец будет двигаться вверх-вниз.
	};

	enum Row
	{
		ROW_ANY, // Обозначет любой ряд
		ROW_LOWEST, // Ряд ниже нижнего
		ROW_LOWER, // Нижний ряд
		ROW_MIDDLE, // Основной ряд
		ROW_UPPER, // Верхний ряд
		ROW_HIGHEST, // Выше верхнего, там обычно располагаются цифры
	};

	enum Column
	{
		COLUMN_ANY, // Обозначет любой столбец
		COLUMN_2LEFT, // Колонка дальше, чем левая. Пример: клавиши Ctrl+SD на моем эргодоксе
		COLUMN_LEFT, // Колонка слева. Пример: у левого мизинца клавиши Caps, Tab, Shift
		COLUMN_MIDDLE, // Основная колонка. Тут находятся все клавиши для среднего, безымянного пальцев.
		COLUMN_RIGHT, // Колонка справа. Пример: указательный палец левой руки, мизинец правой руки
		COLUMN_2RIGHT // Колонка дальше, чем правая. Пример: клавиши Ctrl+CVX на моем эргодоксе
	};

	//-------------------------------------------------------------------------
	typedef int KeyPos; // Позиция клавиши на клавиатуре
	struct Tap {KeyPos key; Press press;};
	struct Key {int layer; KeyPos key;}; // Позиция клавиши в раскладке, включает в себя слой, на котором нажимается клавиша
	typedef std::vector<KeyPos> Accord; // Это аккорд, и здесь выбрано именно расположение клавиши без слоя, потому что в реальности нажатие аккорда представляет из себя только нажатие реальных клавиш, слои и всё такое дело есть только в голове человека и в микросхеме клавиатуры.

	typedef std::vector<KeyPos> KeyPoses;
	typedef std::vector<Tap> Taps;
	typedef std::vector<Key> Keys;
	typedef std::vector<Accord> Accords;

	//-------------------------------------------------------------------------
	class Keyboard
	{
	public:
		struct KeyboardKey
		{
			double x, y;
			double xsize, ysize;
			double angle;

			Hand hand;
			Finger finger;
			Row row;
			Column column;
		};

		Keyboard();
		Keyboard(std::string name, 
				 const std::vector<KeyboardKey>& keys);	

		int size(void) const;

		std::string getName(void) const;

		Hand	getHand(KeyPos key) const;
		Finger	getFinger(KeyPos key) const;
		Row		getRow(KeyPos key) const;
		Column	getColumn(KeyPos key) const;

		// Методы, чтобы нарисовать клавиатуру
		std::pair<int, int>	getPos(KeyPos key) const;
		std::pair<int, int>	getSize(KeyPos key) const;
		double				getAngle(KeyPos key) const;

		// По руке, пальцу, ряду, колонкам можно получить все клавиши
		KeyPoses getKeys(Hand hand,
						 Finger finger,
						 Row row,
						 Column column) const;

		std::vector<KeyboardKey> getKeyboardInnerFormat(void) const;

	private:
		std::string 				m_name;
		std::vector<KeyboardKey> 	m_keys;
	};

	//-------------------------------------------------------------------------
	/** Показывает физическое состояние печатальщика и клавиатуры. Оно характеризуется занятыми пальцами, а так же текущий слой у клавиатуры. */
	class PhysicalState
	{
	public:
		PhysicalState(int defaultLayer);

		// В единственный следующий раз выдаст вместо слоя заданное число
		void addOneTap(int layer);

		// Возвращает текущий слой
		int getCurrentLayer(void) const;

		// Говорит занят ли палец, и какой именно клавишей он занят
		std::optional<KeyPos> isFingerBusy(Hand hand, Finger finger) const;

		// Занимает палец на заданной клавише, при этом включается слой layer
		void busyFinger(Hand hand, Finger finger, KeyPos key, int layer);
		
		// Отпускает палец. При этом из стека пропадает слой, связанный с этим пальцем
		bool unbusyFinger(Hand hand, Finger finger);

	private:
		typedef int FingerId;
		std::map<std::pair<Hand, Finger>, std::optional<KeyPos>> busyFingers;
		std::vector<std::pair<FingerId, int>> 					 layerStack;
		mutable bool 											 isOneTap;
		int 													 oneTapLayer;
	};

	//-------------------------------------------------------------------------
	class Layout : public Keyboard
	{
	public:
		struct LayoutSymbols
		{
			Key key;
			std::wstring symbols;
		};

		Layout();
		Layout(const Keyboard& keyboard,
			   const std::vector<LayoutSymbols>& symbols);

		// Возвращает набор символов, которые будут при нажатии определенной клавиши на определенном слое
		const std::wstring& getSymbols(Key key) const; 
		const Keys& 		getKeys(wchar_t letter) const;

		/** Возвращает минимальную последовательность нажатий, чтобы включить некоторый слой. */
		const std::vector<KeyPoses>& getLayerKeys(int currentLayer, int toLayer) const;

		/** Эмулирует нажатие клавиш, возвращает какой текст напишется в итоге. */
		/** Особенность: тут передаются только нажатия и зажатия клавиш, тут может быть зажата какая-то клавиша, чтобы включить слой. */
		std::wstring typeTaps(const Taps& taps, PhysicalState& state) const;
		/** Особенность: тут передаются только одиночные нажатия, переключение слоя не учитывается. Соответственно не может быть занятых пальцев, текущего слоя и других частей физического состояния. */
		std::wstring typeKeys(const Keys& keys) const;

		const std::vector<LayoutSymbols>& getLayoutInnerFormat(void) const;

	private:
		std::vector<LayoutSymbols> 								m_symbols;
		std::vector<std::vector<std::wstring>> 					m_layerMas;
		std::map<wchar_t, Keys>									m_keyMap;
		std::map<std::pair<int, int>, std::vector<KeyPoses>>	m_layerMap;
	};

	//-------------------------------------------------------------------------
	// Раскладывает первую минимально возможную часть на все возможные варианты нажатия клавиш со слоём в заданной раскладке. Именно здесь выбирается сколько символов будет набрано за одну итерацию алгоритма. Потому что только в этой итерации можно рассмотреть все варианты, чтобы среди них выбрать самый оптимальный, который не повлияет на следующие итерации.
	std::vector<Keys> decomposeToKeys(
		const Layout& layout,
		const std::wstring& text,
		int maxOneHandSize,
		int& symbolsCount
	);

	// Раскладывает нажатие клавиш со слоём на все возможные нажатия кнопок на клавиатуре. Тут уже учитывается физическое состояние (слой и занятые пальцы).
	std::vector<Taps> decomposeToTaps(
		const Layout& layout,
		const Keys& keys,
		PhysicalState& state
	);

	// Раскладывает клавиши на одной руке на все возможные аккорды
	std::vector<Accords> decomposeOneHandAccords(
		const Keyboard& keyboard, 
		const KeyPoses& keyPoses
	);

	// Раскладывает нажатие клавиш на клавиатуре на все возможные варианты набора этого аккордами
	std::vector<Accords> decomposeToAccords(
		const Keyboard& keyboard, 
		const KeyPoses& keyPoses
	);

	//-------------------------------------------------------------------------
	/** Набирает текст прямо сейчас. */
	class Typer
	{
	public:
		Typer(const Layout& layout);

		/** Возвращает номер самого оптимального варианта набора аккордов. 
		Пояснение: результат каждого набора является одним и тем же набором символов (но может и ещё переключить слой, заставляя некоторый палец зажимать клавишу для переключения слоя). Учитывается предыдущее состояние пальцев, что задается функцией type. */
		virtual int getOptimalAccords(const std::vector<Accords>& variants) const = 0;

		/** Набирает заданное число аккордов. Результатом является то, что набираются некоторые символы, при этом у наборщика двигаются руки в необходимую позицию. Так же возвращает время набора. */
		virtual double type(const Accords& accords) = 0;
	protected:
		Layout m_layout;
	};

	//-------------------------------------------------------------------------
	/** Наборщик, который максимально разгоняется во время чередования. Не оценивает насколько сложно перестроить пальцы для чередования. */
	class TyperAlternationLover : public Typer
	{
	public:
		TyperAlternationLover(const Layout& layout);
		int getOptimalAccords(const std::vector<Accords>& variants) const;
		double type(const Accords& accords);
	};

	//-------------------------------------------------------------------------
	/** Наборщик, который максимально разгоняется, когда есть аккорды длиной 2. Причем эти аккорды должны чередоваться между рук. Не оценивает удобство аккордов. */
	class TyperAccordsLover : public Typer
	{
	public:
		TyperAccordsLover(const Layout& layout);
		int getOptimalAccords(const std::vector<Accords>& variants) const;
		double type(const Accords& accords);
	};

	//-------------------------------------------------------------------------
	/** Реальный наборщик, которому и чередование и аккорды важны; учитывает положение рук после предыдущей итерации написания текста; учитывает удобство аккордов; учитывает удобство чередования. */
	class RealTyper : public Typer
	{
	public:
		RealTyper(const Layout& layout);
		int getOptimalAccords(const std::vector<Accords>& variants) const;
		double type(const Accords& accords);
	};

	//-------------------------------------------------------------------------
	void saveToFile(const Keyboard& keyboard, std::string keyboardFile);
	void readFromFile(Keyboard& keyboard, std::string keyboardFile);

	void saveToFile(const Layout& layout, std::string layoutFile);
	void readFromFile(Layout& layout, std::string layoutFile, std::string keyboardFile);

	//-------------------------------------------------------------------------
	/*
	drawKey(); // Можно выбирать стиль: просто квадрат, квадрат со скругленными краями, квадрат, как на сайте эргодокса, как макеты улучшения эргодокса
	// Рисует исключительно клавиатуру
	void draw(twg::ImageBase& img, 
			  Keyboard& keyboard, 
			  const FindBorders& brd, 
			  //..
			  );

	// Рисует исключительно подписи для клавиатуры
	void draw(twg::ImageBase& img, 
			  Layout& keyboard, 
			  const FindBorders& brd, 
			  //..
			  );
	*/

	//-------------------------------------------------------------------------
	std::optional<int> getLayer(wchar_t symbol); // Определят номер слоя по специальным символам

}