from multiprocessing import Process
import socket
from listener import *
from mysql_conf import *


sessions = []

def autorization(user):
	data = user.recv(2048)
	msg = data.decode('utf-8')
	msg = msg.split(":")
	if msg[0] == 'login':
		login = msg[1]
		password = msg[2]
		user_param = check_user(login, password)
		if user_param:
			msg = 'success:' + str(user_param['login']) + ":" + str(user_param['money']) + ":" + str(user_param['score'])
			msg = str(len(msg)) + msg
			print(f'Login success: {msg}')
			user.send(msg.encode('utf-8'))
		else:
			text = 'fail'
			msg = str(len(text)) + text;
			user.send(msg.encode('utf-8'))
			autorization(user)
	elif msg[0] == 'reg':
		login = msg[1]
		password = msg[2]
		user_param = create_user(login, password)
		msg = '07success'
		user.send(msg.encode('utf-8'))
	else:
		text = 'nofunc'
		msg = str(len(text)) + text;
		user.send(msg.encode('utf-8'))
		autorization(user)
	sessions.append(user_param)
	print(f"Слушаю {user}")
	listen_user(user, user_param)
		

def start_server():
	while True:
		user_socket, address = server.accept()
		print("new user connected!")

		autorization_user = Process(target=autorization, args=(user_socket,))
		autorization_user.start()


if __name__ == '__main__':
	server = socket.socket(

		socket.AF_INET,
		socket.SOCK_STREAM,

	)
	server.bind(('0.0.0.0', 1222))
	server.listen()

	start_server()
