#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <SFML/Graphics.hpp>
#include <time.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include<sstream>

SOCKET Connection;
using namespace sf;
using std::string;
using std::to_string;

std::string name, money, score, user_textures;

auto connect() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		//exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1222);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		//std::cout << "Error: failed connect to server.\n";
		//exit(1);
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

const int M = 20; // высота игрового поля
const int N = 10; // ширина игрового поля
std::string indicator = "f", tetramino_texture = "tiles";

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

bool check_textures(char num) {
	std::cout << "num = " << num << ", user_textures = " << user_textures << std::endl;
	std::string sep = "|";
	int i = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = user_textures.find(sep)) != std::string::npos) {
		token = user_textures.substr(0, pos);
		if (token == num)
			std::cout << "token = " << token << std::endl;
			return 1;
	}
	if (user_textures == num)
		return 1;
	std::cout << "return 0;" << std::endl;
	return 0;
}

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

void start_game(RenderWindow& window) {

	// Создание и загрузка текстуры
	Texture texture, texture_background, texture_frame, texture_menu;
	texture.loadFromFile("images/" + tetramino_texture + ".png");
	texture_background.loadFromFile("images/background.png");
	texture_frame.loadFromFile("images/frame.png");

	// Создание спрайта
	Sprite sprite(texture), sprite_background(texture_background), sprite_frame(texture_frame);

	// Вырезаем из спрайта отдельный квадратик размером 18х18 пикселей
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// Переменные для горизонтального перемещения и вращения
	int dx = 0; bool rotate = 0; int colorNum = 1; bool beginGame = true; int n = rand() % 7; int score_now = 0; bool exit = 0;

	// Переменные для таймера и задержки
	float timer = 0, delay = 0.3;

	// Часы (таймер)
	Clock clock;
	beginGame = true;
	std::string temp = "";
	// Главный цикл приложения. Выполняется, пока открыто окно
	while ((window.isOpen()) && (exit == 0))
	{
		
		char msg[64];
		recv(Connection, msg, sizeof(msg), NULL);
		std::string msg_str = decoding(msg);
		if (temp != "") {
			if (msg_str[0] == temp) {}
			else if (msg_str[0] == indicator) {
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
				temp = indicator;
			}
			else {
				temp = msg_str[0];
			}
		}


		if (msg_str[0] == indicator) {
			std::cout << "Xog HaIII, indicator = " << indicator << ", temp = " << temp << std::endl;
			temp = indicator;
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Up) rotate = true;
					else if (event.key.code == Keyboard::Left) dx = -1;
					else if (event.key.code == Keyboard::Right) dx = 1;
				//else if (event.key.code == Keyboard::Escape) return 1;
			}

			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }

			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

			if (rotate)
			{
				Point p = a[1];
				for (int i = 0; i < 4; i++)
				{
					int x = a[i].y - p.y;
					int y = a[i].x - p.x;
					a[i].x = p.x - x;
					a[i].y = p.y + y;
				}
				if (!check()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

			}

			//----ОПУСКАЕМ ВНИЗ----//
			if (timer > delay)
			{
				for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
				std::string msg_str = "";
				if (!check())
				{
					msg_str += "2|";
				}
				else {
					msg_str += indicator + "|";
				}
				for (int i = 0; i < 4; i++)
				{
					msg_str += std::to_string(a[i].x);
					msg_str += "|";
					msg_str += std::to_string(a[i].y);
					msg_str += "|";
				}
				//std::cout << "msg_str = " << msg_str << std::endl;
				send(Connection, msg_str.c_str(), msg_str.length(), NULL);

				if (!check())
				{
					for (int i = 0; i < 4; i++)
						field[b[i].y][b[i].x] = colorNum;
					colorNum = 1 + rand() % 7;
					n = rand() % 7;
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[n][i] % 2; //std::cout << "NEW a[i].x = " << a[i].x << std::endl;
						a[i].y = figures[n][i] / 2; //std::cout << "NEW a[i].y = " << a[i].y << std::endl;
					}
					for (int i = 0; i < 4; i++)
						if (field[a[i].y][a[i].x]) {
							exit = 1;
						}
					if (exit == 1) {
						char msg[64];
						recv(Connection, msg, sizeof(msg), NULL);
						msg_str = "3:" + to_string(score_now); // + счет данного игрока
						send(Connection, msg_str.c_str(), msg_str.length(), NULL);
						recv(Connection, msg, sizeof(msg), NULL);

						msg_str = decoding(msg);
						std::string sep = ":";
						int i = 0;
						size_t pos = 0;
						std::string token;
						while ((pos = msg_str.find(sep)) != std::string::npos) {
							token = msg_str.substr(0, pos);
							if (i == 0) {}
							else {
								if (i == 1) { name = token; }
								if (i == 2) { money = token; }
							}
							i++;
							msg_str.erase(0, pos + sep.length());
						}
						score = msg_str;
						exit = 1;
					}
				}
				timer = 0;
			}
			else {

				std::string msg_str = indicator + "|";

				for (int i = 0; i < 4; i++)
				{
					msg_str += std::to_string(a[i].x);
					msg_str += "|";
					msg_str += std::to_string(a[i].y);
					msg_str += "|";
				}
				send(Connection, msg_str.c_str(), msg_str.length(), NULL);
			}

			// ---- СТРОКА ---- //
			int k = M - 1;
			for (int i = M - 1; i > 0; i--)
			{
				int count = 0;
				for (int j = 0; j < N; j++)
				{
					if (field[i][j]) count++;
					field[k][j] = field[i][j];
				}
				if (count < N)
					k--;
				else
					score_now++;
			}

			// Проверка на конец игры
			/*if (endGame) {
				beginGame = true;
			}*/

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

			window.clear(Color::White);
			window.draw(sprite_background);
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 0) continue;
					sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					sprite.setPosition(j * 18, i * 18);
					sprite.move(28, 31);
					window.draw(sprite);
				}

			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				sprite.setPosition(a[i].x * 18, a[i].y * 18);
				sprite.move(28, 31);
				window.draw(sprite);
			}
			window.draw(sprite_frame); // Рамка
			window.display();
		}
		else if (msg_str == "e|0|0|0|0|0|0|0|0") {
			exit = 1;
			msg_str = "3:" + to_string(score_now);
			send(Connection, msg_str.c_str(), msg_str.length(), NULL);
			recv(Connection, msg, sizeof(msg), NULL);

			msg_str = decoding(msg);
			std::string sep = ":";
			int i = 0;
			size_t pos = 0;
			std::string token;
			while ((pos = msg_str.find(sep)) != std::string::npos) {
				token = msg_str.substr(0, pos);
				if (i == 0) {}
				else {
					if (i == 1) { name = token; }
					if (i == 2) { money = token; }
				}
				i++;
				msg_str.erase(0, pos + sep.length());
			}
			score = msg_str;
		}
		else {
			std::cout << "Xog He HaIII, indicator = " << indicator << ", temp = " << temp << std::endl;
			beginGame = false; // ходим 2ми - точно не начало игры

			// -- Принимаем и расшифровываем ход и местоположение фигуры -- //
			std::string s = msg_str;
			std::string sep = "|";
			int i = 0, ik = 0;
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(sep)) != std::string::npos) {
				token = s.substr(0, pos);
				if (i == 0) {}
				else if (i % 2 == 0) { //token Y
					a[ik].y = atoi(token.c_str());
					ik++;
				}
				else //token X
					a[ik].x = atoi(token.c_str());
		
				i++;
				s.erase(0, pos + sep.length());
			}

			// -- проверяем упала ли фигурка, проверяем строку -- //

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Escape) break;
			}

			for (int i = 0; i < 4; i++)
				b[i] = a[i];
			if (!check())
			{
				for (int i = 0; i < 4; i++)
					field[a[i].y-1][a[i].x] = colorNum;
				colorNum = 1 + rand() % 7;
				n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			// ---- СТРОКА ---- //
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
			msg_str = "OK";
			send(Connection, msg_str.c_str(), msg_str.length(), NULL);

			//----ОТРИСОВКА----//

			window.clear(Color::White);
			window.draw(sprite_background);
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 0) continue;
					sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					sprite.setPosition(j * 18, i * 18);
					sprite.move(28, 31);
					window.draw(sprite);
				}

			for (int i = 0; i < 4; i++)
			{
				sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				sprite.setPosition(a[i].x * 18, a[i].y * 18);
				sprite.move(28, 31);
				window.draw(sprite);
			}
			window.draw(sprite_frame);
			window.display();
		}
	}

	field[M][N] = { 0 };
}

void find_game(RenderWindow& window) {
	Texture texture_menu, textbox;
	texture_menu.loadFromFile("images/menu.png");
	textbox.loadFromFile("images/login/textbox.png");

	std::string msg_str = "findgame";
	send(Connection, msg_str.c_str(), msg_str.length(), NULL);
	char msg[64];
	recv(Connection, msg, sizeof(msg), NULL);
	msg_str = decoding(msg);

	if (msg_str == "noplayers"){
		sf::Font font;
		font.loadFromFile("images/normal.otf");

		Texture t_rooms, t_connect, t_newgame;
		t_rooms.loadFromFile("images/rooms.png");
		t_newgame.loadFromFile("images/newgame.png");
		Sprite s_rooms(t_rooms);
		Sprite s_newgame(t_newgame);
		s_rooms.setPosition(10, 10);
		s_newgame.setPosition(170, 435);

		Sprite s_textbox(textbox);
		sf::Event event;
		bool exit = 0;
		while (window.isOpen() && exit == 0) {
			window.clear(Color::White);

			window.draw(s_rooms);
			window.draw(s_newgame);

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Escape) exit = 1;
			}

			int menuNum = 0;
			if (IntRect(170, 435, 140, 35).contains(Mouse::getPosition(window))) { menuNum = 11; }

			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (menuNum == 11) {
					indicator = "f";
					std::string msg_str = "newgame:0|0|0|0|0|0|0|0";

					send(Connection, msg_str.c_str(), msg_str.length(), NULL);

					Texture t_newgame_back;
					t_newgame_back.loadFromFile("images/newgame_back.png");
					Sprite s_newgame_back(t_newgame_back);
					s_newgame_back.setPosition(0, 0);
					window.draw(s_newgame_back);
					window.display();

					char msg[64];
					recv(Connection, msg, sizeof(msg), NULL);
					msg_str = decoding(msg);
					if (msg_str == "start") {
						start_game(window);
						exit = 1;
					}
				}
			}

			window.display();
		}
	}
	else {
		sf::Font font;
		font.loadFromFile("images/normal.otf");
		
		Texture t_rooms, t_connect, t_newgame;
		t_rooms.loadFromFile("images/rooms.png");
		t_connect.loadFromFile("images/connect.png");
		t_newgame.loadFromFile("images/newgame.png");
		Sprite s_rooms(t_rooms);
		Sprite s_connect(t_connect);
		Sprite s_newgame(t_newgame);
		s_rooms.setPosition(10, 10);
		s_connect.setPosition(10, 435);
		s_newgame.setPosition(170, 435);

		sf::Clock blink_text_clock;
		static sf::Time text_effect_time;
		static bool show_cursor = true;

		Sprite s_textbox(textbox);
		int x = 0, y = 0, userNum = 1;
		sf::Event event;
		bool exit = 0;
		while (window.isOpen() && exit == 0) {
			window.clear(Color::White);

			window.draw(s_rooms);
			window.draw(s_connect);
			window.draw(s_newgame);

			if ((x > 0) && (y > 0)) {
				RectangleShape rectangle(Vector2f(300, 40));
				rectangle.move(x, y);
				rectangle.setFillColor(Color(175, 180, 240));
				window.draw(rectangle);
			}

			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Escape) exit = 1;
			}

			std::string s = msg_str;
			std::string sep = " ";
			std::string sep2 = ":";
			int i = 0;
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(sep)) != std::string::npos) {
				token = s.substr(0, pos);

				std::string t_user = token.substr(0, token.find(sep2));
				std::string t_id = token.substr(token.find(sep2) + 1, -1);

				sf::Text TextBox_user(t_user, font, 14);
				TextBox_user.setFillColor(sf::Color::Black);
				TextBox_user.setPosition(15, 70 + 40 * i);
				window.draw(TextBox_user);

				sf::Text TextBox_id(t_id, font, 14);
				TextBox_id.setFillColor(sf::Color::Black);
				TextBox_id.setPosition(150, 70 + 40 * i);
				window.draw(TextBox_id);

				i++;
				s.erase(0, pos + sep.length());
			}

			int menuNum = 0;
			if (IntRect(10, 60, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 1; }
			if (IntRect(10, 100, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 2; }
			if (IntRect(10, 140, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 3; }
			if (IntRect(10, 180, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 4; }
			if (IntRect(10, 220, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 5; }
			if (IntRect(10, 260, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 6; }
			if (IntRect(10, 300, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 7; }
			if (IntRect(10, 340, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 8; }
			if (IntRect(10, 380, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 9; }
			if (IntRect(10, 435, 140, 35).contains(Mouse::getPosition(window))) { menuNum = 10; }
			if (IntRect(170, 435, 140, 35).contains(Mouse::getPosition(window))) { menuNum = 11; }
			
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (menuNum == 1) {
					x = 10;
					y = 60;
					userNum = menuNum;
				}
				else if (menuNum == 2) {
					x = 10;
					y = 100;
					userNum = menuNum;
				}
				else if (menuNum == 3) {
					x = 10;
					y = 140;
					userNum = menuNum;
				}
				else if (menuNum == 4) {
					x = 10;
					y = 180;
					userNum = menuNum;
				}
				else if (menuNum == 5) {
					x = 10;
					y = 220;
					userNum = menuNum;
				}
				else if (menuNum == 6) {
					x = 10;
					y = 260;
					userNum = menuNum;
				}
				else if (menuNum == 7) {
					x = 10;
					y = 300;
					userNum = menuNum;
				}
				else if (menuNum == 8) {
					x = 10;
					y = 340;
					userNum = menuNum;
				}
				else if (menuNum == 9) {
					x = 10;
					y = 380;
					userNum = menuNum;
				}
				else if (menuNum == 10) { // connect
					indicator = "t";
					std::string msg_str_n = "connect:";
					msg_str_n += std::to_string(userNum);
					send(Connection, msg_str_n.c_str(), msg_str_n.length(), NULL);
					char msg[64];
					recv(Connection, msg, sizeof(msg), NULL);
					msg_str = decoding(msg);
					if (msg_str == "start") {
						start_game(window);
						exit = 1;
					}
				}
				else if (menuNum == 11) { // create room
					indicator = "f";
					std::string msg_str = "newgame:0|0|0|0|0|0|0|0";

					send(Connection, msg_str.c_str(), msg_str.length(), NULL);

					Texture t_newgame_back;
					t_newgame_back.loadFromFile("images/newgame_back.png");
					Sprite s_newgame_back(t_newgame_back);
					s_newgame_back.setPosition(0, 0);
					window.draw(s_newgame_back);
					window.display();

					char msg[64];
					recv(Connection, msg, sizeof(msg), NULL);
					msg_str = decoding(msg);
					if (msg_str == "start") {
						start_game(window);
						exit = 1;
					}
				}
			}

			window.display();
		}
	}
}

void menu_shop(RenderWindow& window) {
	
	Texture textbox, texture_menu;
	Sprite sprite_menu_back(texture_menu);
	sprite_menu_back.setTextureRect(IntRect(0, 0, 320, 480));
	textbox.loadFromFile("images/login/textbox.png");
	texture_menu.loadFromFile("images/menu.png");
	std::string msg_str = "shop";
	send(Connection, msg_str.c_str(), msg_str.length(), NULL);
	char msg[256];
	recv(Connection, msg, sizeof(msg), NULL);
	msg_str = decoding(msg);
	sf::Font font;
	font.loadFromFile("images/normal.otf");

	Texture t_buy, t_setup;
	t_buy.loadFromFile("images/buy.png");
	t_setup.loadFromFile("images/setup.png");
	Sprite s_buy(t_buy), s_setup(t_setup);
	s_buy.setPosition(10, 390);
	s_setup.setPosition(170, 390);

	Sprite s_textbox(textbox);
	int x = 0, y = 0, textureNum;
	sf::Event event;
	bool exit = 0;
	while (window.isOpen() && exit == 0) {

		sf::Text tx_info("user: " + name + "   money: " + money + "   score: " + score, font, 10);
		tx_info.setFillColor(sf::Color::White);
		tx_info.setPosition(0, 0);

		window.clear(Color::White);
		window.draw(sprite_menu_back);
		window.draw(tx_info);
		window.draw(s_buy);
		window.draw(s_setup);

		if ((x > 0) && (y > 0)) {
			RectangleShape rectangle(Vector2f(300, 40));
			rectangle.move(x, y);
			rectangle.setFillColor(Color(175, 180, 240));
			window.draw(rectangle);
		}

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape) exit = 1;
		}

		std::string s = msg_str;
		std::string sep = " ";
		std::string sep2 = "_";
		int i = 0;
		size_t pos = 0;
		std::string token, link, price;
		while ((pos = s.find(sep)) != std::string::npos) {
			token = s.substr(0, pos);
			link = "images/" + token.substr(0, token.find(sep2)) + ".png";
			price = token.substr(token.find(sep2) + 1, -1);

			try {
				Texture texture;
				texture.loadFromFile(link);
				Sprite s_texture(texture);
				s_texture.setPosition(20, 43 + 40 * i);
				sf::Text priceBox(price, font, 14);

				if (check_textures(link[12])) {
					priceBox.setFillColor(sf::Color::Green);
				}
				else {
					priceBox.setFillColor(sf::Color::Yellow);
				}
				
				priceBox.setPosition(260, 43 + 40 * i);

				RectangleShape line(Vector2f(320, 2.f));
				line.setFillColor(sf::Color::White);
				line.move(0, 70 + 40 * i);

				window.draw(line);
				window.draw(s_texture);
				window.draw(priceBox);
			}
			catch (const std::exception& e) {

			}

			i++;
			s.erase(0, pos + sep.length());
		}

		int menuNum = 0;
		if (IntRect(20, 30, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 1; }
		if (IntRect(20, 70, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 2; }
		if (IntRect(20, 110, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 3; }
		if (IntRect(20, 150, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 4; }
		if (IntRect(20, 190, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 5; }
		if (IntRect(20, 230, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 6; }
		if (IntRect(20, 270, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 7; }
		if (IntRect(20, 310, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 8; }
		if (IntRect(20, 350, 300, 40).contains(Mouse::getPosition(window))) { menuNum = 9; }
		if (IntRect(10, 390, 140, 35).contains(Mouse::getPosition(window))) { menuNum = 10; }
		if (IntRect(170, 390, 140, 35).contains(Mouse::getPosition(window))) { menuNum = 11; }

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (menuNum == 1) {
				x = 10;
				y = 30;
				textureNum = menuNum;
				std::cout << textureNum << std::endl;
			}
			else if (menuNum == 2) {
				x = 10;
				y = 70;
				textureNum = menuNum;
			}
			else if (menuNum == 3) {
				x = 10;
				y = 110;
				textureNum = menuNum;
			}
			else if (menuNum == 4) {
				x = 10;
				y = 150;
				textureNum = menuNum;
			}
			else if (menuNum == 5) {
				x = 10;
				y = 190;
				textureNum = menuNum;
			}
			else if (menuNum == 6) {
				x = 10;
				y = 230;
				textureNum = menuNum;
			}
			else if (menuNum == 7) {
				x = 10;
				y = 270;
				textureNum = menuNum;
			}
			else if (menuNum == 8) {
				x = 10;
				y = 310;
				textureNum = menuNum;
			}
			else if (menuNum == 9) {
				x = 10;
				y = 350;
				textureNum = menuNum;
			}
			else if (menuNum == 10) { // купить
				std::cout << "menuNum = 10, userNum = " << textureNum << std::endl;
				//std::string msg_str = "findgame:" + userNum;
				//send(Connection, msg_str.c_str(), msg_str.length(), NULL);
				//char msg[64];
				//recv(Connection, msg, sizeof(msg), NULL);
				//msg_str = decoding(msg);
				//if (msg_str == "start") {
				//	start_game(window);
				//	exit = 1;
				//}
			}
			else if (menuNum == 11) { // установить
				std::cout << "menuNum = 11, userNum = " << textureNum << std::endl;
				if (check_textures(textureNum)) {
					tetramino_texture = "tiles" + std::to_string(textureNum);
				}
				
			}
		}
		window.display();
	}
}

void menu_rating(RenderWindow& window) {

	Texture textbox, texture_menu;
	Sprite sprite_menu_back(texture_menu);
	sprite_menu_back.setTextureRect(IntRect(0, 0, 320, 480));
	textbox.loadFromFile("images/login/textbox.png");
	texture_menu.loadFromFile("images/menu.png");
	std::string msg_str = "rating";
	send(Connection, msg_str.c_str(), msg_str.length(), NULL);
	char msg[256];
	recv(Connection, msg, sizeof(msg), NULL);
	msg_str = decoding(msg);
	sf::Font font;
	font.loadFromFile("images/normal.otf");

	Sprite s_textbox(textbox);
	int x = 0, y = 0, textureNum;
	sf::Event event;
	bool exit = 0;
	while (window.isOpen() && exit == 0) {
		window.clear(Color::White);
		window.draw(sprite_menu_back);

		sf::Text userBox("USER", font, 14);
		sf::Text scoreBox("SCORE", font, 14);
		userBox.setFillColor(sf::Color::White);
		userBox.setPosition(20, 5);
		scoreBox.setFillColor(sf::Color::White);
		scoreBox.setPosition(250, 5);

		window.draw(userBox);
		window.draw(scoreBox);

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape) exit = 1;
		}

		std::string s = msg_str;
		std::string sep = " ";
		std::string sep2 = ":";
		int i = 0;
		size_t pos = 0;
		std::string token, t_user, t_score;
		while ((pos = s.find(sep)) != std::string::npos) {
			token = s.substr(0, pos);
			t_user = token.substr(0, token.find(sep2));
			t_score = token.substr(token.find(sep2) + 1, -1);

			try {
				sf::Text userBox(t_user, font, 13);
				sf::Text scoreBox(t_score, font, 13);
				userBox.setFillColor(sf::Color::White);
				userBox.setPosition(20, 25 + 20 * i);
				scoreBox.setFillColor(sf::Color::White);
				scoreBox.setPosition(250, 25 + 20 * i);

				RectangleShape line(Vector2f(320, 2.f));
				line.setFillColor(sf::Color::White);
				line.move(0, 25 + 20 * i);

				window.draw(line);
				window.draw(userBox);
				window.draw(scoreBox);
			}
			catch (const std::exception& e) {

			}
			i++;
			s.erase(0, pos + sep.length());
		}
		window.display();
	}
}

void menu_help(RenderWindow& window) {

	Texture texture_menu;
	Sprite sprite_menu_back(texture_menu);
	sprite_menu_back.setTextureRect(IntRect(0, 0, 320, 480));
	texture_menu.loadFromFile("images/menu_help.png");
	
	sf::Font font;
	font.loadFromFile("images/normal.otf");

	sf::Event event;
	bool exit = 0;
	while (window.isOpen() && exit == 0) {
		window.clear(Color::White);
		window.draw(sprite_menu_back);

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape) exit = 1;
		}
		
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
	int itemMenu = 0;
	sprite_menu_back.setPosition(0, 0);
	sprite_menu_game.setPosition(80, 85);
	sprite_menu_mag.setPosition(60, 175);
	sprite_menu_stat.setPosition(30, 245);
	sprite_menu_help.setPosition(60, 315);

	while (isMenu == 1)
	{
		sf::Font font;
		font.loadFromFile("images/normal.otf");

		sf::Text tx_info("user: " + name + "   money: " + money + "   score: " + score, font, 10);
		tx_info.setFillColor(sf::Color::White);
		tx_info.setPosition(0,0);

		sprite_menu_game.setColor(Color::White);
		sprite_menu_mag.setColor(Color::White);
		sprite_menu_stat.setColor(Color::White);
		sprite_menu_help.setColor(Color::White);
		itemMenu = 0;
		window.clear(Color(129, 181, 221));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (IntRect(80, 85, 240, 80).contains(Mouse::getPosition(window))) { sprite_menu_game.setColor(Color::Blue); itemMenu = 1; } // Играть
		if (IntRect(60, 175, 260, 50).contains(Mouse::getPosition(window))) { sprite_menu_mag.setColor(Color::Blue); itemMenu = 2; } // Магазин
		if (IntRect(30, 245, 290, 50).contains(Mouse::getPosition(window))) { sprite_menu_stat.setColor(Color::Blue); itemMenu = 3; } // Статистика
		if (IntRect(60, 315, 260, 50).contains(Mouse::getPosition(window))) { sprite_menu_help.setColor(Color::Blue); itemMenu = 4; } // Помощь

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (itemMenu == 1) {
				find_game(window);
			} 
			else if (itemMenu == 2) { 
				menu_shop(window);
			}
			else if (itemMenu == 3) { 
				menu_rating(window);
			}
			else if (itemMenu == 4) {
				menu_help(window);
			}

		}

		window.draw(sprite_menu_back);
		window.draw(tx_info);
		window.draw(sprite_menu_game);
		window.draw(sprite_menu_mag);
		window.draw(sprite_menu_stat);
		window.draw(sprite_menu_help);

		window.display();
	}
}

string repl(string data, string sub) {
	int i = 0;
	while (data[i] != NULL) {
		if (data[i] == sub) {
			data.erase(i, 1);
			if ((i - 1) >= 0) i--;
		}
		else i++;
	}
	return data;
}

bool logining(string login) {
	int login_success = 0;
	send(Connection, login.c_str(), login.length(), NULL);

	char msg[64];
	recv(Connection, msg, sizeof(msg), NULL);
	try {
		std::string msg_str = decoding(msg);

		std::string sep = ":";
		int i = 0;
		size_t pos = 0;
		std::string token;
		while ((pos = msg_str.find(sep)) != std::string::npos) {
			token = msg_str.substr(0, pos);
			if ((i == 0) && (login_success == 0)) {
				if (token == "success") {
					login_success = 1;
				}
				else {
					break;
				}
			}
			else {
				if (i == 1) { name = token; }
				if (i == 2) { money = token; }
				if (i == 3) { score = token; }
			}
			i++;
			msg_str.erase(0, pos + sep.length());
		}
		user_textures = msg_str;

		std::cout << "name: " << name << std::endl;
		std::cout << "money: " << money << std::endl;
		std::cout << "score: " << score << std::endl;

		if (login_success == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	catch (const std::exception& e) {
		return 0;
	}

}

void menu_connect(RenderWindow& window) {
	sf::Image WindowIcon;
	if (!WindowIcon.loadFromFile("images/icon.png"))
		std::cout << "cant load texture from file " << std::endl;
	else
		window.setIcon(128, 128, WindowIcon.getPixelsPtr());
	sf::Font font;
	sf::Font& refFont = font;
	if (!font.loadFromFile("images/normal.otf")) {
		std::cout << "cant load font" << std::endl;
	}
	Texture t_Background, under_bg;
	t_Background.loadFromFile("images/login/bg.png");
	Sprite s_bg(t_Background),
		s_under_bg(t_Background);
	//============================================================
	Texture t_b_left, t_b_right, t_w_left, t_w_right, t_textbox, t_reg, t_vhod;
	t_b_left.loadFromFile("images/login/b_left.png");
	t_b_right.loadFromFile("images/login/b_right.png");
	t_w_left.loadFromFile("images/login/w_left.png");
	t_w_right.loadFromFile("images/login/w_right.png");
	t_textbox.loadFromFile("images/login/textbox.png");
	t_reg.loadFromFile("images/login/reg.png");
	t_vhod.loadFromFile("images/login/vhod.png");
	Texture t_B_enter, t_B_registration, t_B_login, t_B_password, t_B_enter_click, t_B_registration_click;
	t_B_enter.loadFromFile("images/login/login_enter.png");
	t_B_registration.loadFromFile("images/login/Reg_enter.png");
	t_B_login.loadFromFile("images/login/mail.png");
	t_B_password.loadFromFile("images/login/password.png");
	Sprite s_b_left(t_b_left),
		s_b_right(t_b_right),
		s_w_left(t_w_left),
		s_w_right(t_w_right),
		s_textbox1(t_textbox),
		s_textbox2(t_textbox),
		s_reg(t_reg),
		s_vhod(t_vhod),
		s_B_enter_reg,
		s_B_enter(t_B_enter),
		s_B_registration(t_B_registration),
		s_B_login(t_B_login),
		s_B_password(t_B_password);
	Sprite B_left, B_right;
	//------------------------------------------------------------
	s_bg.setPosition(0, 0);
	s_under_bg.setPosition(0, 50);

	int current_button_loginOrReg = 0;
	B_left = s_b_right;
	B_right = s_b_left;
	B_left.setPosition(2.f, 2.f);
	B_right.setPosition(160.f, 2.f);

	s_textbox1.setPosition(10.f, 100.f); // Поле "Логин"
	s_textbox2.setPosition(10.f, 160.f); // Поле "Пароль"

	s_B_enter_reg.setPosition(10.f, 420.f); // Кнопка "Вход/Регистрация"

	s_reg.setPosition(50.f, 25.f); // Переключатель "Регистрация"
	s_vhod.setPosition(220.f, 25.f); // Переключатель "Вход"


	//------------------------------------------------------------
	//TextBox
	std::srand((sf::Mouse::getPosition(window).x + sf::Mouse::getPosition(window).y));

	std::string login = "", password = "";
	sf::Text TextBox_login(login, font, 24);
	sf::Text TextBox_password(password, font, 24);
	TextBox_login.setFillColor(sf::Color::Black);
	TextBox_password.setFillColor(sf::Color::Black);
	//TextBox_login.setStyle(sf::Text::Bold);
	//TextBox_password.setStyle(sf::Text::Bold);
	//-----------
	sf::Clock blink_text_clock;
	static sf::Time text_effect_time;
	static bool show_cursor = true;
	//-----------
	TextBox_login.setPosition(15, 105); // Текст логин
	TextBox_password.setPosition(15, 165); // Текст пароль
	//-----------
	//------------------------------------------------------------

	int TailleWindow[4] = { static_cast<int>(window.getSize().x),
							static_cast<int>(window.getSize().y),
							sf::Mouse::getPosition(window).x,
							sf::Mouse::getPosition(window).y
	};

	int what_do = 0;
	int login_pass_ip_port = 0;

	int reg_ok = 0;
	int login_ok = 0;
	int end = 0;
	sf::Event event;

	while (window.isOpen() && end == 0) {

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				end = 1;
				break;
			case sf::Event::TextEntered:
				if (std::isprint(event.text.unicode)) {
					if (login_pass_ip_port == 0)
						login += event.text.unicode;
					else if (login_pass_ip_port == 1)
						password += event.text.unicode;
					login = repl(login, "\t");
					login = repl(login, "\n");

					password = repl(password, "\t");
					password = repl(password, "\n");
				}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) {
					end = 1;
					break;
				}
				if (event.key.code == sf::Keyboard::Enter) {
					int answer = 0;
					sf::Clock clock, pause;
					float ftime = clock.getElapsedTime().asSeconds();
					clock.restart();
					while (answer == 0 && clock.getElapsedTime().asSeconds() < 6) {
						if (pause.getElapsedTime().asSeconds() >= 1) {
							answer = logining("login:" + login + ":" + password);
							pause.restart();
						}
					}
					if (answer == 1) {
						// запомнить параметры
					}
					login_ok = answer;
				}
				if (event.key.code == sf::Keyboard::BackSpace) {
					if (login_pass_ip_port == 0) {
						if (!login.empty())
							login.pop_back();
					}
					else if (login_pass_ip_port == 1) {
						if (!password.empty())
							password.pop_back();
					}
					
					login = repl(login, "\t");
					login = repl(login, "\n");

					password = repl(password, "\t");
					password = repl(password, "\n");
				}
				if (event.key.code == sf::Keyboard::Return) {
					/*
					if (login_pass_ip_port == 0)
						login += '\n';
					else
						password += '\n';
					*/
				}
				if (event.key.code == sf::Keyboard::Tab) {
					if (login_pass_ip_port == 5)
						login_pass_ip_port = 0;
					else if (login_pass_ip_port == 4)
						login_pass_ip_port = 5;
					else if (login_pass_ip_port == 3)
						login_pass_ip_port = 4;
					else if (login_pass_ip_port == 2)
						login_pass_ip_port = 3;
					else if (login_pass_ip_port == 1)
						login_pass_ip_port = 2;
					else if (login_pass_ip_port == 0)
						login_pass_ip_port = 1;
					login = repl(login, "\t");
					login = repl(login, "\n");

					password = repl(password, "\t");
					password = repl(password, "\n");
				}
				break;
			case Event::MouseButtonPressed://MouseButtonPressed//MouseButtonReleased
				/*
				if (event.key.code == Mouse::Left) {
					std::cout << "x=" << sf::Mouse::getPosition(window).x << std::endl;
					std::cout << "y=" << sf::Mouse::getPosition(window).y << std::endl;
					TailleWindow[2] = sf::Mouse::getPosition(window).x;
					TailleWindow[3] = sf::Mouse::getPosition(window).y;
				}
				*/
				//isMenu = false;
				break;
			}
			/*
			if (event.type == Event::Closed) {
				//Обработка события при нажатии на закрыть окно
				end = 1;
			}
			*/
		}



		if (what_do == 0) {


			B_left.setTexture(t_b_left);
			B_right.setTexture(t_w_right);

			s_B_enter_reg.setTexture(t_B_enter);


		}
		else {
			B_left.setTexture(t_w_left);
			B_right.setTexture(t_b_right);

			s_B_enter_reg.setTexture(t_B_registration);
		}

		int menuNum = 0;
		if (IntRect(2.f, 2.f, 160.f, 49.f).contains(Mouse::getPosition(window))) { menuNum = 1; }
		if (IntRect(160.f, 2.f, 318.f, 49.f).contains(Mouse::getPosition(window))) { menuNum = 2; }

		if (IntRect(5.f, 100.f, 310.f, 35.f).contains(Mouse::getPosition(window))) { menuNum = 3; }
		if (IntRect(5.f, 160.f, 310.f, 35.f).contains(Mouse::getPosition(window))) { menuNum = 4; }

		if (IntRect(5.f, 420.f, 310.f, 40.f).contains(Mouse::getPosition(window))) { menuNum = 9; }

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (menuNum == 1) { what_do = 1; }
			if (menuNum == 2) { what_do = 0; }
			if (menuNum == 3) { login_pass_ip_port = 0; }
			if (menuNum == 4) { login_pass_ip_port = 1; }
			if (menuNum == 9) {
				if (what_do == 0) {
					//s_B_enter_reg.setTexture(t_B_enter_click);
					//login
					int answer = 0;
					sf::Clock clock, pause;
					float ftime = clock.getElapsedTime().asSeconds();
					clock.restart();
					while (answer == 0 && clock.getElapsedTime().asSeconds() < 6) {
						if (pause.getElapsedTime().asSeconds() >= 1) {
							answer = logining("login:" + login + ":" + password);
							pause.restart();
						}
					}
					if (answer == 1) {
						// запомнить параметры
					}
					login_ok = answer;
				}
				else {
					//s_B_enter_reg.setTexture(t_B_registration_click);
					//register
					int answer = 0;
					sf::Clock clock, pause;
					float ftime = clock.getElapsedTime().asSeconds();
					clock.restart();

					while (answer == 0 && clock.getElapsedTime().asSeconds() < 6) {
						if (pause.getElapsedTime().asSeconds() >= 1) {
							answer = logining("reg:" + login + ":" + password);
							pause.restart();
						}
					}
					login_ok = answer;
					//reg_ok = answer;


				}

			}
		}


		text_effect_time += blink_text_clock.restart();
		if (text_effect_time >= sf::seconds(0.5f)) {
			show_cursor = !show_cursor;
			text_effect_time = sf::Time::Zero;
		}

		if (reg_ok == 1) {
			TextBox_login.setFillColor(sf::Color::Green);
			TextBox_password.setFillColor(sf::Color::Green);
			TextBox_login.setString((show_cursor ? "Ok>|" : "Ok>[") + login + (show_cursor ? "|" : "]"));
			TextBox_password.setString((show_cursor ? "Ok>|" : "Ok>[") + password + (show_cursor ? "|" : "]"));
		}
		else if (reg_ok == 0) {
			TextBox_login.setFillColor(sf::Color::Black);
			TextBox_password.setFillColor(sf::Color::Black);
			if (login_pass_ip_port == 0) {
				TextBox_login.setString((show_cursor ? "Login:_" : "Login: ") + login);
			}
			else if (login_pass_ip_port == 1) {
				TextBox_password.setString((show_cursor ? "Pass:_" : "Pass: ") + password);
			}
		}


		window.draw(s_bg);
		window.draw(s_under_bg);

		window.draw(B_left);
		window.draw(B_right);
		window.draw(s_textbox1);
		window.draw(s_textbox2);
		window.draw(s_B_enter_reg);
		window.draw(s_reg);
		window.draw(s_vhod);
		window.draw(TextBox_login);
		window.draw(TextBox_password);
		window.display();
		if (login_ok == 1) {
			end = 1;
			menu(window);
		}
	}
	window.close();
}

int main()
{
	Connection = connect();
	srand(time(0));

	RenderWindow window(VideoMode(320, 480), "Tetris PLUS");
	window.setFramerateLimit(60);
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)menu, NULL, NULL, NULL);
	menu_connect(window);
	
	return 0;
}