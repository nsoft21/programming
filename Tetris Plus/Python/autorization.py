

def autorization(user):
	data = user.recv(2048)
	try:
		msg = data.decode('utf-8')
		if msg.find('login:'):
			msg = msg.split(":")
			login = msg[1]
			password = msg[2]
			user_param = check_user(login, password)
			if user_param:
				pass
			else:
				text = 'fail'
				msg = str(len(text)) + text;
				user.send(msg.encode('utf-8'))
				autorization()
		elif msg.find('reg:'):
			msg = msg.split(":")
			login = msg[1]
			password = msg[2]
			user_param = create_user(login, password)
		else:
			return 0
		listen_user(user, user_param)
	except:
		return 0