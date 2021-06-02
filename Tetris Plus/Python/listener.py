from find_by_key import *
from mysql_conf import *
from storage import *

free_rooms = []
id_s = 0
indicator = 'f'
opponent = 0

def send_rooms(user):
	global free_rooms
	free_rooms = find_rooms()
	if free_rooms == False:
		msg = 'noplayers'
	else:
		msg = ''
		i = 0
		for room in free_rooms:
			if i>8:
				continue
			text = str(room["user1"]) + ':' + str(room["id"]) + ' '
			msg = msg + text
			i = i + 1
	msg = str(len(msg)) + msg;
	user.send(msg.encode('utf-8')) # Отправляем всех возможных соперников


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


def new_game(user1):
	global id_s
	update_db_param(f"UPDATE users SET find_game = 'True' WHERE users.login = '{user1}'")
	id_s = insert_into_rooms(user1, 'find')


def game(user, user_param, swap):
	global id_s
	this.swap = swap
	print(f"В in_game игрок: {user_param['login']}")
	k = this.swap[str(id_s)][0]
	print(f'k = {k}')
	# 0,1 - для решения чей ход
	# 2   - для смены ходящего
	# 3   - для окончания игры

	while k != 3:

		msg = str(this.swap[str(id_s)][0]) + str(this.swap[str(id_s)][1])
		user.send(msg.encode('utf-8'))
		data = user.recv(2048)
		msg = data.decode('utf-8')
		if msg[0] == indicator:
			print(f'msg[0] = indicator')
			print(f'msg[1:] = {msg[1:]}')
			this.swap[str(id_s)] = [indicator, msg[1:]]
		elif msg[0] == 2:
			this.swap[str(id_s)] = [opponent, msg[1:]]
		elif msg[0] == 3:
			k = 3
	# Проверяем что нажимает пользователь


def listen_user(user, user_param, swap):
	global id_s, free_rooms, opponent
	this.swap = swap
	while True:
		data = user.recv(2048)
		if user_param['find_game'] == "False":
			try:
				msg = data.decode('utf-8')
				msg = msg.split(':')
				if msg[0] == 'findgame':
					send_rooms(user)
					continue
				elif msg[0] == 'newgame':
					new_game(user_param['login'])
					this.swap[id_s] = [randint(0, 1), msg[1]]

					while True:
						if check_opponent(id_s) == "True":
							break
						
					update_db_param(f"UPDATE users SET find_game = 'in_game' WHERE users.login = '{user_param['login']}'")
					print('В NEWGAME прошла проверка')
					user_param['find_game'] = 'in_game'
					indicator = 'f'
					opponent = 1
					msg = '05start'
					user.send(msg.encode('utf-8'))
					game(user, user_param, this.swap)
					# Изменить значение 'find_game' на True
				elif msg[0] == 'rating':
					send_users_rating(user)
				elif msg[0] == 'shop':
					send_textures(user)
				elif msg[0] == 'connect':
					id_s = free_rooms[int(msg[1]) - 1]['id']
					query = f"UPDATE users SET find_game = 'in_game' WHERE users.login = '{user_param['login']}'"
					print(f'query1 = {query}')
					update_db_param(query)
					query = f"UPDATE rooms SET user2 = '{user_param['login']}' WHERE rooms.id = {id_s}"
					print(f'query2 = {query}')
					update_db_param(query)
					print('Должна быть запись в БД о user2')
					user_param['find_game'] = 'in_game'
					indicator = 't'
					opponent = 0
					msg = '05start'
					print(f"connect: {msg}")
					user.send(msg.encode('utf-8'))
					print(f"in_game")
					game(user, user_param, this.swap)
				else:
					print("Зашли в ELSE:")
					print(msg)
			except:
				msg = data.decode('utf-8')
				print(f"EXCEPT: {msg}")
		elif user_param['find_game'] == "True":
			print('continue')
			continue
			
			
			
