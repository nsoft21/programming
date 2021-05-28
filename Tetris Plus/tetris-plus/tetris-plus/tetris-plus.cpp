#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <SFML/Graphics.hpp>
#include <time.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>

SOCKET Connection;
using namespace sf;

auto connect() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1222);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		//std::cout << "Error: failed connect to server.\n";
		exit(1);
	}
	//std::cout << "Connected!\n";

	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	return Connection;
}

std::string decoding(char msg[]) {
	int a = msg[0] - '0';
	int b = msg[1] - '0';
	int size_msg = a * 10 + b;

	if ((size_msg < 0) && (size_msg > 99)) {
		return "error";
	}

	std::string s(msg);
	std::string msg_str = s.substr(2, size_msg);

	return msg_str;
}

void tokenize(std::string s, std::string del = " ")
{
	
}

const int M = 20; // высота игрового поля
const int N = 10; // ширина игрового поля

int field[M][N] = { 0 }; // игровое поле

// Массив фигурок-тетрамино
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
	int x, y;
} a[4], b[4];


// Проверка на выход за границы игрового поля
bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

	return 1;

};

bool endGame()
{
	for (int i = 0; i < N; i++)
	{
		if (field[1][i])
		{
			return true;
		}
	}
}

int start_game(RenderWindow& window) {

	//Отправить запрос на сервер, получить ответ после ожидания игрока, согласиться что готов и запустить игру



	// Создание и загрузка текстуры
	Texture texture, texture_background, texture_frame, texture_menu;
	texture.loadFromFile("images/tiles.png");
	texture_background.loadFromFile("images/background.png");
	texture_frame.loadFromFile("images/frame.png");

	// Создание спрайта
	Sprite sprite(texture), sprite_background(texture_background), sprite_frame(texture_frame);

	// Вырезаем из спрайта отдельный квадратик размером 18х18 пикселей
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// Переменные для горизонтального перемещения и вращения
	int dx = 0; bool rotate = 0; int colorNum = 1; bool beginGame = true; int n = rand() % 7;

	// Переменные для таймера и задержки
	float timer = 0, delay = 0.3;

	// Часы (таймер)
	Clock clock;


	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Получаем время, прошедшее с начала отсчета, и конвертируем его в секунды
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет окно закрыть?
			if (event.type == Event::Closed)
				// тогда закрываем его
				window.close();

			// Была нажата кнопка на клавиатуре?
			if (event.type == Event::KeyPressed)
				// Эта кнопка – стрелка вверх?…
				if (event.key.code == Keyboard::Up) rotate = true;
			// …или же стрелка влево?…
				else if (event.key.code == Keyboard::Left) dx = -1;
			// …ну тогда может это стрелка вправо?
				else if (event.key.code == Keyboard::Right) dx = 1;
				else if (event.key.code == Keyboard::Escape) return 1;
		}

		// Нажали кнопку "вниз"? Ускоряем падение тетрамино
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//// Горизонтальное перемещение ////
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }

		// Вышли за пределы поля после перемещения? Тогда возвращаем старые координаты 
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];


		//// Вращение ////
		if (rotate)
		{
			Point p = a[1]; // задаем центр вращения
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y; //y-y0
				int y = a[i].x - p.x; //x-x0
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			// Вышли за пределы поля после поворота? Тогда возвращаем старые координаты 
			if (!check()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

		}

		//// Движение тетрамино вниз (Тик таймера) ////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;
				colorNum = 1 + rand() % 7;
				n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}

			}
			timer = 0;

		}

		//----ПРОВЕРКА ЛИНИИ----//
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

		// Проверка на конец игры
		/*if (endGame) {
			beginGame = true;
		}*/

		// Первое появление тетрамино на поле?
		if (beginGame)
		{
			beginGame = false;
			n = rand() % 7;
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		dx = 0; rotate = 0; delay = 0.3;

		//----ОТРИСОВКА----//

		// Задаем цвет фона - белый
		window.clear(Color::White);
		window.draw(sprite_background);
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31); // смещение
				window.draw(sprite);
			}

		for (int i = 0; i < 4; i++)
		{
			// Разукрашиваем тетрамино
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));

			// Устанавливаем позицию каждого кусочка тетрамино
			sprite.setPosition(a[i].x * 18, a[i].y * 18);

			sprite.move(28, 31); // смещение

			// Отрисовка спрайта
			window.draw(sprite);
		}
		// Отрисовка фрейма
		window.draw(sprite_frame);

		// Отрисовка окна
		window.display();
	}

}

void menu(RenderWindow& window) {
	Texture texture_menu;
	texture_menu.loadFromFile("images/menu.png");
	Sprite sprite_menu_back(texture_menu), sprite_menu_game(texture_menu), sprite_menu_mag(texture_menu), sprite_menu_stat(texture_menu), sprite_menu_help(texture_menu);
	sprite_menu_back.setTextureRect(IntRect(0, 0, 320, 480));
	sprite_menu_game.setTextureRect(IntRect(450, 0, 160, 70));
	sprite_menu_mag.setTextureRect(IntRect(410, 65, 200, 50));
	sprite_menu_stat.setTextureRect(IntRect(350, 120, 260, 50));
	sprite_menu_help.setTextureRect(IntRect(410, 175, 200, 50));
	int isMenu = 1;
	int menuNum = 0;
	sprite_menu_back.setPosition(0, 0);
	sprite_menu_game.setPosition(80, 85);
	sprite_menu_mag.setPosition(60, 175);
	sprite_menu_stat.setPosition(30, 245);
	sprite_menu_help.setPosition(60, 315);

	while (isMenu == 1)
	{
		sprite_menu_game.setColor(Color::White);
		sprite_menu_mag.setColor(Color::White);
		sprite_menu_stat.setColor(Color::White);
		sprite_menu_help.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (IntRect(80, 85, 240, 80).contains(Mouse::getPosition(window))) { sprite_menu_game.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(60, 175, 260, 50).contains(Mouse::getPosition(window))) { sprite_menu_mag.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(30, 245, 290, 50).contains(Mouse::getPosition(window))) { sprite_menu_stat.setColor(Color::Blue); menuNum = 3; }
		if (IntRect(60, 315, 260, 50).contains(Mouse::getPosition(window))) { sprite_menu_help.setColor(Color::Blue); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {
				
				isMenu = 2;
			} //если нажали первую кнопку, то выходим из меню 
			//if (menuNum == 2) { window.draw(); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(sprite_menu_back);
		window.draw(sprite_menu_game);
		window.draw(sprite_menu_mag);
		window.draw(sprite_menu_stat);
		window.draw(sprite_menu_help);

		window.display();
	}
	while (isMenu == 2) {
		std::string msg_text = "findgame";
		send(Connection, msg_text.c_str(), msg_text.length(), NULL);

		char msg[64];
		recv(Connection, msg, sizeof(msg), NULL); // Ждем списка игроков

		window.clear();
		Font font;
		Text header("Открытые комнаты", font, 18);
		Text new_game_text("Новая игра", font, 14);

		header.setPosition(140, 15);
		new_game_text.setPosition(140, 450);
		window.draw(header);
		window.draw(new_game_text);

		std::string msg_str = decoding(msg);
		if (msg_str == "noplayers") {

		}
		else {
			// Разбить строки
			int i = 1;
			int start = 0;
			std::string del = " ";
			int end = msg_str.find(del);
			while (end != -1) {
				Text users(msg_str.substr(start, end - start), font, 12);
				users.setPosition(120, 30*i);
				window.draw(users);
				start = end + del.size();
				end = msg_str.find(del, start);
			}
			std::cout << msg_str.substr(start, end - start);
		}



		if (msg_str == "start") { // Обрабатываем список и выводим
			start_game(window);
		}

		
		window.display();
	}
}


bool login(RenderWindow& window, std::string login, std::string pass = "") {
	std::string user_data;
	if (pass == "") {
		user_data = "login:" + login;
	}
	else {
		user_data = "login:" + login + ":" + pass;
	}

	send(Connection, user_data.c_str(), user_data.length(), NULL);

	char msg[64];
	recv(Connection, msg, sizeof(msg), NULL);
	try {
		std::string msg_str = decoding(msg);
		if (msg_str == "success") {
			menu(window);
		}
		std::cout << msg_str << std::endl;
		return(true);
	}
	catch (const std::exception& e) {

	}

}


void autorization(RenderWindow& window) {
	// Проверить наличие файла с учетными данными, если есть, взять оттуда и отправить на сервер
	// если нет - открыть меню для входа/регистрации

	std::ifstream fin("cppstudio.txt");

	if (!fin.is_open()) {
		// если файл не открыт
		// сделать поле регистрации
		window.close();
		exit(1);
	}
	else
	{
		std::string s;
		fin >> s;
		fin.close();
		login(window, s);
	}


}


int main()
{
	Connection = connect();
	srand(time(0));

	RenderWindow window(VideoMode(320, 480), "Tetris PLUS");
	window.setFramerateLimit(60);
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)menu, NULL, NULL, NULL);
	autorization(window);
	
	return 0;
}