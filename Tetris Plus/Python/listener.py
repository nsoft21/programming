from find_by_key import *
from mysql_conf import *


def send_free_users(user):
	free_users = find_user_by_key()
	if len(free_users) == 0:
		msg = 'noplayers'
	else:
		msg = str(len(free_users))
		i = 0
		for users in free_users:
			if i>8:
				continue
			text = users["login"] + ':' + str(users["score"]) + ' '
			msg = msg + text
	msg = str(len(msg)) + msg;
	print(f'Отправляю комнаты: {msg}')
	user.send(msg.encode('utf-8')) # Отправляем всех возможных соперников
	return free_users


def send_textures(user):
	texture_list = get_textures()
	if len(texture_list) == 0:
		msg = 'notexture'
	else:
		msg = ''
		i = 0
		for texture in texture_list:
			if i>8:
				continue
			text = texture["link"] + '_' + str(texture["price"]) + ' '
			msg = msg + text
	msg = str(len(msg)) + msg;
	user.send(msg.encode('utf-8')) # Отправляем текстуры


def send_users_rating(user):
	users_list = get_users_rating()
	msg = ''
	i = 0
	for users in users_list:
		if i>8:
			continue
		text = str(users["login"]) + ':' + str(users["score"]) + ' '
		msg = msg + text
	msg = str(len(msg)) + msg;
	user.send(msg.encode('utf-8')) # Отправляем таблицу рейтинга


def listen_user(user, user_param):
	while True:
		print("Жду запроса LISTEN USER: ")
		data = user.recv(2048)
		if user_param['find_game'] == "False":
			print(f"Получил {data.decode('utf-8')}, зашел в findgame")
			try:
				msg = data.decode('utf-8')
				if msg == 'findgame':
					print("Формирую и отправляю комнаты")
					free_user = send_free_users(user)
					continue
				elif msg == 'newgame':
					print('newgame')
					msg = msg.split(':')
					print(msg)
					# Изменить значение 'find_game' на True
				elif msg == 'rating':
					send_users_rating(user)
				elif msg == 'shop':
					send_textures(user)
				elif msg == 'connect':
					msg = msg.split(':')
					free_user[msg[1]]
				else:
					print("Зашли в ELSE:")
					print(msg)
			except:
				msg = data.decode('utf-8')
				print(f"EXCEPT: {msg}")
		elif user_param['find_game'] == "True":
			print('continue')
			continue
		elif user_param['find_game'] == 'in_game':
			# Проверяем что нажимает пользователь
			data = user.recv(2048)
			msg = data.decode('utf-8')
			print("Зашли в ELIF:")
			print(msg)
