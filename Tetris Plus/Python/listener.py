from find_by_key import *
from mysql_conf import *
from storage import *

free_rooms = []
id_s = 0
indicator = 'f'
opponent = 't'

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


def buy_textures(user, num):
	texture_list = get_textures()
	# texture_list[num]
	#	for texture in texture_list:
	#		text = texture["link"] + '_' + str(texture["price"]) + ' '
	#		msg = msg + text
	#msg = str(len(msg)) + msg;
	#user.send(msg.encode('utf-8')) # Отправляем текстуры


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


def end_of_game(user_param, score, id_s):
	user_param['score'] = user_param['score'] + score
	user_param['money'] = user_param['money'] + score
	user_param['find_game'] = "False"
	update_db_param(f"UPDATE users SET score = '{user_param['score']}', money = '{user_param['money']}', find_game = 'False' WHERE users.login = '{user_param['login']}'")
	update_db_param(f"DELETE FROM rooms WHERE id = {id_s}")

	msg = 'endgame:' + str(user_param['login']) + ":" + str(user_param['money']) + ":" + str(user_param['score'])
	msg = str(len(msg)) + msg;
	return msg


def game(user, user_param, swap):
	global id_s
	this.swap = swap
	k = this.swap[str(id_s)][0]
	# 0,1 - для решения чей ход
	# 2   - для смены ходящего
	# 3   - для окончания игры

	while k != 3:

		msg = str(this.swap[str(id_s)][0]) + '|' + str(this.swap[str(id_s)][1])
		msg = str(len(msg)) + msg
		user.send(msg.encode('utf-8'))
		data = user.recv(2048)
		msg = data.decode('utf-8')
		if (msg[0] == indicator) or (msg[0] == opponent):
			this.swap[str(id_s)] = [msg[0], msg[2:]]
		elif msg[0] == "2":
			this.swap[str(id_s)] = [opponent, msg[2:]]
		elif msg[0] == "3":
			msg = msg.split(':')
			score = int(msg[1])
			this.swap[str(id_s)] = ["e", "0|0|0|0|0|0|0|0"]
			msg = end_of_game(user_param, score, id_s)
			print(msg)
			user.send(msg.encode('utf-8'))
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
				elif msg[0] == 'newgame': # -------------------------------- NEW GAME -------------------------------------- #
					new_game(user_param['login'])
					if randint(0, 1) == 1:
						this.swap[id_s] = ['t', msg[1]]
					else:
						this.swap[id_s] = ['f', msg[1]]

					while True:
						if check_opponent(id_s) == "True":
							break
						
					update_db_param(f"UPDATE users SET find_game = 'in_game' WHERE users.login = '{user_param['login']}'")
					update_db_param(f"UPDATE rooms SET status = 'in_game' WHERE id = {id_s}")
					print('В NEWGAME прошла проверка')
					user_param['find_game'] = 'in_game'
					indicator = 'f'
					opponent = 't'
					msg = '05start'
					user.send(msg.encode('utf-8'))
					game(user, user_param, this.swap)
					# Изменить значение 'find_game' на True
				elif msg[0] == 'rating':  # --------------------------------- RATING --------------------------------------- #
					send_users_rating(user)
				elif msg[0] == 'shop':    # ---------------------------------- SHOP ---------------------------------------- #
					print(msg)
					if len(msg) != 1:
						buy_textures(user, msg[1])
					else:
						send_textures(user)
				elif msg[0] == 'connect': # --------------------------------- CONNECT -------------------------------------- #
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
					opponent = 'f'
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
			
			
			
