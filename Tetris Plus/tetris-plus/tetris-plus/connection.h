#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>

SOCKET Connection;

auto connect() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1234);
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

void login(std::string login, std::string pass, std::string hash) {
	if (hash.empty()) { // задано пользователем
		std::string s = "login:" + login + ":" + pass;
		//std::string s = s.length() + s;
		send(Connection, s.c_str(), s.length(), NULL);
	}
	else { // взято из файла
		std::string s = "hash:" + hash;
		send(Connection, s.c_str(), s.length(), NULL);
	}

	char msg[64];
	recv(Connection, msg, sizeof(msg), NULL);
	std::cout << "Login success!" << std::endl;
	try {
		int a = msg[0] - '0';
		int b = msg[1] - '0';
		int size_msg = a * 10 + b;
		std::cout << size_msg << std::endl;
		std::string s(msg);
		std::string msg_str = s.substr(2, size_msg);
		if (msg_str == "start") {
			//start_game();
		}
		std::cout << msg_str << std::endl;
	}
	catch (const std::exception& e) {

	}

}


void registration(RenderWindow& window) {
	// Проверить наличие файла с учетными данными, если есть, взять оттуда и отправить на сервер
	// если нет - открыть меню для входа/регистрации

	std::ifstream fin("cppstudio.txt");

	if (!fin.is_open()) {
		// если файл не открыт
		window.close();
	}
	else
	{
		std::string s;
		int i = 0;
		std::string hash;
		fin >> hash;
		fin.close();

		login("", "", hash);
		// Отправить данные пользователя на сервер для проверки

	}


}
