from find_by_key import *

def listen_user(user, user_param):
	while True:
		if user_param['find_game'] == False:
			data = user.recv(2048)
			try:
				msg = data.decode('utf-8')
				if msg.find('findgame'):
					send_free_users(user);
					continue
				elif msg.find('newgame'):
					msg = msg.split(':')
					print(msg)
					# Изменить значение 'find_game' на True
				elif msg.find('rating'):
					pass
				elif msg.find('shop'):	
					pass
				else:
					print(msg)
			except:
				pass
		elif user_param['find_game'] == True:
			continue
		elif user_param['find_game'] == 'in_game':
			# Проверяем что нажимает пользователь
			data = user.recv(2048)
			msg = data.decode('utf-8')
			print(msg)

def send_free_users(user):
	free_users = find_all_by_key(sessions, "find_game", True)
	msg = ''
	for users in free_users:
		text = users[0] + ':' + users[1][name] + ':' + users[1][score] + ' '
		msg = msg + text
	msg = str(len(msg)) + msg;
	user.send(msg.encode('utf-8')) # Отправляем всех возможных соперников