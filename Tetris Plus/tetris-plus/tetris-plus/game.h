#include <SFML/Graphics.hpp>

using namespace sf;

const int M = 20; // ������ �������� ����
const int N = 10; // ������ �������� ����

int field[M][N] = { 0 }; // ������� ����

// ������ �������-���������
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

// �������� �� ����� �� ������� �������� ����
bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

	return 1;

};

// �������� �� ����� ����
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

// �������� ������ ����
int start_game(RenderWindow& window) {

	//��������� ������ �� ������, �������� ����� ����� �������� ������, ����������� ��� ����� � ��������� ����



	// �������� � �������� ��������
	Texture texture, texture_background, texture_frame, texture_menu;
	texture.loadFromFile("images/tiles.png");
	texture_background.loadFromFile("images/background.png");
	texture_frame.loadFromFile("images/frame.png");

	// �������� �������
	Sprite sprite(texture), sprite_background(texture_background), sprite_frame(texture_frame);

	// �������� �� ������� ��������� ��������� �������� 18�18 ��������
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// ���������� ��� ��������������� ����������� � ��������
	int dx = 0; bool rotate = 0; int colorNum = 1; bool beginGame = true; int n = rand() % 7;

	// ���������� ��� ������� � ��������
	float timer = 0, delay = 0.3;

	// ���� (������)
	Clock clock;


	// ������� ���� ����������. �����������, ���� ������� ����
	while (window.isOpen())
	{
		// �������� �����, ��������� � ������ �������, � ������������ ��� � �������
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// ������������ ������� ������� � �����
		Event event;
		while (window.pollEvent(event))
		{
			// ������������ ����� �� �������� � ����� ���� �������?
			if (event.type == Event::Closed)
				// ����� ��������� ���
				window.close();

			// ���� ������ ������ �� ����������?
			if (event.type == Event::KeyPressed)
				// ��� ������ � ������� �����?�
				if (event.key.code == Keyboard::Up) rotate = true;
			// ���� �� ������� �����?�
				else if (event.key.code == Keyboard::Left) dx = -1;
			// ��� ����� ����� ��� ������� ������?
				else if (event.key.code == Keyboard::Right) dx = 1;
				else if (event.key.code == Keyboard::Escape) return 1;
		}

		// ������ ������ "����"? �������� ������� ���������
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//// �������������� ����������� ////
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }

		// ����� �� ������� ���� ����� �����������? ����� ���������� ������ ���������� 
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];


		//// �������� ////
		if (rotate)
		{
			Point p = a[1]; // ������ ����� ��������
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y; //y-y0
				int y = a[i].x - p.x; //x-x0
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			// ����� �� ������� ���� ����� ��������? ����� ���������� ������ ���������� 
			if (!check()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

		}

		//// �������� ��������� ���� (��� �������) ////
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

		//----�������� �����----//
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

		// �������� �� ����� ����
		/*if (endGame) {
			beginGame = true;
		}*/

		// ������ ��������� ��������� �� ����?
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

		//----���������----//

		// ������ ���� ���� - �����
		window.clear(Color::White);
		window.draw(sprite_background);
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31); // ��������
				window.draw(sprite);
			}

		for (int i = 0; i < 4; i++)
		{
			// ������������� ���������
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));

			// ������������� ������� ������� ������� ���������
			sprite.setPosition(a[i].x * 18, a[i].y * 18);

			sprite.move(28, 31); // ��������

			// ��������� �������
			window.draw(sprite);
		}
		// ��������� ������
		window.draw(sprite_frame);

		// ��������� ����
		window.display();
	}

}

// �������� ����
void menu(RenderWindow& window) {
	Texture texture_menu;
	texture_menu.loadFromFile("images/menu.png");
	Sprite sprite_menu_back(texture_menu), sprite_menu_game(texture_menu), sprite_menu_mag(texture_menu), sprite_menu_stat(texture_menu), sprite_menu_help(texture_menu);
	sprite_menu_back.setTextureRect(IntRect(0, 0, 320, 480));
	sprite_menu_game.setTextureRect(IntRect(450, 0, 160, 70));
	sprite_menu_mag.setTextureRect(IntRect(410, 65, 200, 50));
	sprite_menu_stat.setTextureRect(IntRect(350, 120, 260, 50));
	sprite_menu_help.setTextureRect(IntRect(410, 175, 200, 50));
	bool isMenu = 1;
	int menuNum = 0;
	sprite_menu_back.setPosition(0, 0);
	sprite_menu_game.setPosition(80, 85);
	sprite_menu_mag.setPosition(60, 175);
	sprite_menu_stat.setPosition(30, 245);
	sprite_menu_help.setPosition(60, 315);

	while (isMenu)
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
			if (menuNum == 1) { start_game(window); } //���� ������ ������ ������, �� ������� �� ���� 
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
}
