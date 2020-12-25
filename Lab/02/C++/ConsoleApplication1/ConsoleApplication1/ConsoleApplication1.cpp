﻿#include <iostream>
#include <string>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <map>

using namespace httplib;
using json = nlohmann::json;
std::vector<std::string> cache;
typedef std::map<std::string, std::string> new_Map;

json session_list;
new_Map session_list_map;
bool help_pressed = false;

std::string log_file_name = "logs.txt";
std::ofstream log_file(log_file_name);

json button_speak = { {"title", "Говори"}, {"payload", json::object()}, {"hide", true} };
json button_silent = { {"title", "Молчать"}, {"payload", json::object()}, {"hide", true} };
json buttons_help = {
				{{"title", "Молчать"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Говори"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Помощь"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Добавить в корзину"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Удалить из корзины"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Что в корзине"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Сумма"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Покупка завершена"}, {"payload", json::object()}, {"hide", true}},
				{{"title", "Выход"}, {"payload", json::object()}, {"hide", true}}
};

void findAndRepl(std::string& data, std::string toSearch, std::string replStr) {
	size_t pos = data.find(toSearch);
	while (pos != std::string::npos) {
		data.replace(pos, toSearch.size(), replStr);
		pos = data.find(toSearch, pos + replStr.size());
	}
}

//Формирует json покупок пользователя и отправляет на все webhooks
void webhooks_resp(std::string user_id, json cart) {
	json jcart = json::array();

	for (std::string item : cart) {
		std::string sep = u8R"( )";
		std::string temp;
		size_t sep_size = sep.length();
		json jcart_temp = json::array();

		while (true) {
			temp = item.substr(0, item.find(sep));
			if (temp.size() != 0)			jcart_temp.push_back(temp);
			if (temp.size() == item.size()) break;
			else							item = item.substr(temp.size() + sep_size);
		}

		jcart.push_back({ {"item",jcart_temp[0]}, {"price",jcart_temp[1]} });
	}

	json response = {
		{"user_id", user_id},
		{"check", jcart}
	};

	log_file << response;

	for (std::string link : cache) {
		findAndRepl(link, "https://", "http://");

		if (link[link.size() - 1] != '/')
		{
			link.push_back('/');
		}

		Client cli(link.substr(0, link.find('/', 7)).c_str());
		cli.Post(link.substr(link.find('/', 7), std::string::npos).c_str(), response.dump(), "application/json; charset=UTF-8");
	}
}

// В этой функции формируем JSON для Яндекса
json gen_response(const std::string& text,
	const std::string& tts,
	const json buttons,
	const std::string session_id,
	bool end_session = false
)
{
	json response = {
		{"response", {
			{"buttons", buttons},
			{"end_session", end_session}
		}},
		{"version", "1.0"}
	};

	if (text != "")
	{
		response["response"]["text"] = text;
	}

	if (!session_id.empty() && end_session == false) {
		if (session_list[session_id]["speak"])
		{
			if (tts != "")
			{
				response["response"]["tts"] = tts;
			}
			if (!session_list[session_id]["help"]) {
				response["response"]["buttons"].push_back(button_silent);
			}
		}
		else if (!session_list[session_id]["speak"] && !session_list[session_id]["help"])
		{
			if (!session_list[session_id]["help"]) {
				response["response"]["buttons"].push_back(button_speak);
			}
		}
	}

	return response;
}

// Формируем полный ответ на post запрос от Яндекса
void request_processing(const Request& req, Response& res) {
	json j = json::parse(req.body);
	json str;

	std::string session_id = j["session"]["session_id"];

	if (j["session"]["new"]) {
		std::string text = u8R"(Здравствуйте! Я помогу вам с покупками.)";
		json buttons = {
			{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
		};

		session_list_map.emplace(session_id, "1");
		session_list[session_id] = json::object();
		session_list[session_id]["speak"] = true;
		session_list[session_id]["help"] = false;
		session_list[session_id]["sum"] = "0";
		session_list[session_id]["cart"] = json::array();

		str = gen_response(text, text, buttons, session_id);
		res.set_content(str.dump(), "text/json; charset=UTF-8");
	}
	else {
		bool breaking = 1;
		for (auto session : session_list_map)
		{
			if (session.first == session_id)
			{
				breaking = 0;
			}
		}
		if (breaking == 1) {
			std::string text = u8R"(Ошибка, данные не корректны. Попробуйте еще раз.)";
			json buttons = {
			{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
			};
			str = gen_response(text, text, buttons, session_id, true);
			res.set_content(str.dump(), "text/json; charset=UTF-8");
		}
		// Молчать
		if (j["request"]["nlu"]["tokens"][0] == u8"молчать") {
			if (!session_list[session_id]["help"]) {
				session_list[session_id]["speak"] = false;
				std::string text = u8R"(Молчу, молчу..)";
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Вызовите эту команду чтобы я не разговаривала. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Помощь
		else if (j["request"]["nlu"]["tokens"][0] == u8"помощь") {
			if (!session_list[session_id]["help"]) {
				session_list[session_id]["help"] = true;
				std::string text = u8R"(Корзина. Поможет организовать покупки.
Помощь. Расскажет как этим пользоваться.
Говорить/Молчать. Включает и выключает звук соответственно.
О чём рассказать подробнее?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Помощь расскажет как пользоваться данным навыком. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Говорить
		else if (j["request"]["nlu"]["tokens"][0] == u8"говори") {
			if (!session_list[session_id]["help"]) {
				session_list[session_id]["speak"] = true;
				std::string text = u8R"(Ок, буду говорить)";
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Вызовите эту команду чтобы я снова могла говорить. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Завершение покупки
		else if (j["request"]["nlu"]["tokens"][0] == u8"покупка" || j["request"]["nlu"]["tokens"][0] == u8"завершить" || j["request"]["nlu"]["tokens"][0] == u8"купить") {
			if (!session_list[session_id]["help"]) {
				std::string text = u8R"(Заходите ещё)";
				json buttons = {
				{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};

				webhooks_resp(j["session"]["user"]["user_id"], session_list[session_id]["cart"]);

				json arr = json::object();
				
				for (auto session_old = session_list_map.begin(); session_old != session_list_map.end();)
				{
					if (session_old->first == session_id)
					{
						session_old = session_list_map.erase(session_old);
					}
					else if(session_id != session_old->first){
						arr.push_back(session_list);
						++session_old;
					}
				}
				session_list = arr;

				str = gen_response(text, text, buttons, session_id, true);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Эта команда завершает работу с Вашей корзиной. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Что в корзине
		else if (j["request"]["nlu"]["tokens"][0] == u8"что") {
			if (!session_list[session_id]["help"]) {
				std::string text;
				for (auto item_old : session_list[session_id]["cart"]) {
					text += item_old;
					text += "\n";
				}
				if (text.empty()) {
					text = u8R"(Корзина пуста, Вы ничего не добавили)";
				}
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Запросите эту команду чтобы узнать что уже есть в Вашей корзине. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Добавить в корзину
		else if (j["request"]["nlu"]["tokens"][0] == u8"добавить") {
			if (!session_list[session_id]["help"]) {
				std::string good_name = j["request"]["nlu"]["tokens"][3];
				std::string good_price = j["request"]["nlu"]["tokens"][4];
				std::string good_val = j["request"]["nlu"]["tokens"][5];
				std::string good = good_name + " " + good_price + " " + good_val;

				session_list[session_id]["cart"].push_back(good);

				std::string s = session_list[session_id]["sum"];
				int a = std::stoi(s);
				int i = std::stoi(good_price);
				session_list[session_id]["sum"] = std::to_string(a + i);

				std::string text = u8R"(Ок)";
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Назовите эту команду чтобы я добавила товар в корзину (шаблон: Добавить в корзину "товар" "цена"). О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Удалить из корзины
		else if (j["request"]["nlu"]["tokens"][0] == u8"удалить") {
			if (!session_list[session_id]["help"]) {
				bool is_item = false;
				std::string item_name = j["request"]["nlu"]["tokens"][3];
				std::string item_price = j["request"]["nlu"]["tokens"][4];
				std::string item_val = j["request"]["nlu"]["tokens"][5];
				std::string item = item_name + " " + item_price + " " + item_val;

				json arr = json::array();
				for (auto item_old : session_list[session_id]["cart"]) {
					if (item != item_old) {
						arr.push_back(item_old);
					}
					else {
						is_item = true;
					}
				}
				session_list[session_id]["cart"] = arr;

				std::string s = session_list[session_id]["sum"];
				int a = std::stoi(s);
				int i = std::stoi(item_price);
				session_list[session_id]["sum"] = std::to_string(a - i);

				std::string text;
				if (is_item) {
					text = u8R"(Удалила)";
				}
				else {
					text = u8R"(Такого товара нет в корзине, попробуйте еще раз)";
				}

				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Назовите эту команду чтобы я удалила товар из корзины (шаблон: Удалить из корзины "товар" "цена"). О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Очистить корзину
		else if (j["request"]["nlu"]["tokens"][0] == u8"очистить") {
			if (!session_list[session_id]["help"]) {
				session_list[session_id]["cart"] = json::array();
				std::string text = u8R"(Корзина очищена)";
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				session_list[session_id]["sum"] = "0";
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Вызовите эту команду чтобы я очистила Вашу корзину. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Сумма покупки
		else if (j["request"]["nlu"]["tokens"][0] == u8"сумма") {
			if (!session_list[session_id]["help"]) {
				std::string s = session_list[session_id]["sum"];
				std::string text = u8R"(Сумма Вашей покупки )" + s + u8R"( рублей)";
				json buttons = {
					{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
				};
				str = gen_response(text, text, buttons, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
			else {
				std::string text = u8R"(Вызови эту команду чтобы я назвала сумму Вашей покупки. О чем еще рассказать?)";
				str = gen_response(text, text, buttons_help, session_id);
				res.set_content(str.dump(), "text/json; charset=UTF-8");
			}
		}
		// Выход из помощи
		else if (j["request"]["nlu"]["tokens"][0] == u8"выход") {
			session_list[session_id]["help"] = false;
			std::string text = u8R"(Обращайся еще)";
			json buttons = {
				{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
			};
			str = gen_response(text, text, buttons, session_id);
			res.set_content(str.dump(), "text/json; charset=UTF-8");
		}
		else {
			std::string text = u8R"(Не понимаю, попробуй еще)";
			json buttons = {
				{ {"title", "Помощь"}, {"payload", json::object()}, {"hide", true} }
			};
			str = gen_response(text, text, buttons, session_id);
			res.set_content(str.dump(), "text/json; charset=UTF-8");
		}
	}
}

void update_conf(std::string param, std::string val) {
	std::string conf_file_name = "conf.json";
	std::ifstream conf_file(conf_file_name);
	std::string config;
	json j;

	if (conf_file.is_open())
	{
		conf_file >> j;
		conf_file.close();

		if (param == "del") {
			cache.erase(std::remove(cache.begin(), cache.end(), val), cache.end());
		}
		else if (param == "set") {
			cache.push_back(val);
		}
		json j2;
		j2["webhooks"] = cache;
		std::ofstream conf_file(conf_file_name);
		conf_file << j2;
		conf_file.close();
	}
	else
	{
		log_file << "Error open file 3\n";
	}
}

void check_conf() {
	std::string conf_file_name = "conf.json";
	std::ifstream conf_file(conf_file_name);
	std::string config;
	log_file << u8"Попытка открыть файл...\n";
	if (conf_file.is_open())
	{
		log_file << u8"Открыл!\n";
		json j = json::parse(conf_file);

		for (int i = 0; i < j["webhooks"].size(); i++) {
			cache.push_back(j["webhooks"][i]);
			log_file << j["webhooks"][i] << std::endl;
		}
	}
	else
	{
		log_file << u8"Не получилось, создаю новый...\n";
		std::ofstream conf_file(conf_file_name);
		json j2;
		j2["webhooks"] = json::array();
		conf_file << j2;
		conf_file.close();
		cache = {};
	}
}

void gen_response_webhooks(const Request& req, Response& res) {

	std::string template_file_name = "template.html";
	std::ifstream template_file(template_file_name);
	std::string template_str;

	//Читаем шаблон
	if (template_file.is_open())
	{
		getline(template_file, template_str, '\0');
		template_file.close();
	}
	else
	{
		log_file << "Error open file";
	}

	//Формируем html-шаблон Webhooks
	if (cache.empty()) {
		findAndRepl(template_str, "{webhooks_list}", "");
	}
	else {
		std::string template_file_name = "templ_block.html";
		std::ifstream template_file_block(template_file_name);
		std::string template_block;
		std::string temp;

		for (int i = 0; i < cache.size(); i++) {
			if (template_file_block.is_open())
			{
				getline(template_file_block, template_block, '\0');
				temp = template_block;
			}
			else {
				log_file << "Error open file 2";
			}
			findAndRepl(temp, "{Webhook URL}", cache[i]);

			findAndRepl(template_str, "{webhooks_list}", temp);
		}
		template_file_block.close();
		findAndRepl(template_str, "{webhooks_list}", "");
	}

	res.set_content(template_str, "text/html");

}

void gen_response_webhooks_post(const Request& req, Response& res) {
	if (req.has_param("del")) {
		auto val = req.get_param_value("del");
		update_conf("del", val);
	}
	else if (req.has_param("set")) {
		auto val = req.get_param_value("set");
		update_conf("set", val);
	}
	else {
		//Ошибка
	}
	gen_response_webhooks(req, res);
}

int main() {
	//setlocale(LC_ALL, "ru");
	Server svr;
	check_conf();

	svr.Post("/", request_processing);
	svr.Get("/webhooks", gen_response_webhooks);
	svr.Post("/webhooks", gen_response_webhooks_post);
	std::cout << "Start server... OK\n";
	svr.listen("localhost", 1234);
}