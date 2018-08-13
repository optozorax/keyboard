#pragma once

#include <vector>
#include <string>
#include <optional>

namespace kbd
{

	//-------------------------------------------------------------------------
	typedef wchar_t Symbol;
	typedef vector<symbol> Symbols; // В раскладке одна клавиша может в себе иметь сразу несколько символов
	typedef int KeyPos; // Позиция клавиши на клавиатуре
	struct Key {int layer; KeyPos key;}; // Позиция клавиши в раскладке, включает в себя слой, на котором нажимается клавиша
	typedef std::vector<KeyPos> Accord; // Это аккорд, и здесь выбрано именно расположение клавиши без слоя, потому что в реальности нажатие аккорда представляет из себя только нажатие реальных клавиш, слои и всё такое дело есть только в голове человека и в микросхеме клавиатуры.
	typedef std::vector<Accord> Accords; // Написание некоторый фразы или её части на аккорды.

	//-------------------------------------------------------------------------
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
		pair<int, int>	getPos(KeyPos key) const;
		pair<int, int>	getSize(KeyPos key) const;
		double			getAngle(KeyPos key) const;

		// По руке, пальцу, ряду, колонкам можно получить все клавиши
		std::vector<KeyPos> getKeys(Hand hand,
									Finger finger,
									Row row,
									Column column) const;

		std::vector<KeyboardKey> getKeyboardInnerFormat(void) const;

	private:
		std::string 				m_name;
		std::vector<KeyboardKey> 	m_keys;
	};

	//-------------------------------------------------------------------------
	class Layout : public Keyboard
	{
	public:
		struct LayoutSymbols
		{
			Key key;
			Symbols symbols;
		};

		Layout();
		Layout(const Keyboard& keyboard,
			   const std::vector<LayoutSymbols>& symbols);

		// Возвращает набор символов, которые будут при нажатии определенной клавиши на определенном слое
		Symbols 			getSymbols(Key key) const; 
		std::vector<Key> 	getKeys(wchar_t letter) const;

		std::vector<LayoutSymbols> getLayoutInnerFormat(void) const;

	private:
		std::vector<LayoutSymbols> m_symbols;
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
	/** Возвращает разложение фразы на аккорды, если такое возможно. */
	std::optional<std::vector<Accords>> decomposeToAccords(const Layout& layout, std::wstring str, int maxOneHandSize = 10);

	//-------------------------------------------------------------------------
	/** Оценивает время набора последнего аккорда, учитывая что руки будут перестраиваться, при написании всех предыдущих аккордов. Изначальная позиция - "фыва олдж" на нулевом слое. */
	/** ВНИМАНИЕ!!! Является главным местом реализации текущей модели. Соответственно это максимально необъективное место. */
	double evalTime(const Keyboard& keyboard, const Accords& phrase); 

	/** Раскладывает текст на нажатие аккордов на клавиатуре, причем самым оптимальным образом, чтобы время набора было минимальным. Зависит от evalTime. */
	Accords parseText(const Layout& layout, const std::wstring& text);

	//-------------------------------------------------------------------------
	std::optional<int> getLayer(Symbol symbol); // Определят номер слоя по специальным символам

}