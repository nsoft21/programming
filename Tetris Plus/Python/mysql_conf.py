import pymysql
from pymysql.cursors import DictCursor
from random import randint


def connect():
	connection = pymysql.connect(
	    host='192.168.0.250', #80.245.117.198
	    user='nickwerty5',
	    password='Password123',
	    db='test',
	    charset='utf8mb4',
	    cursorclass=DictCursor
	)
	return connection

def create_user(login = 'niki3', password = '1234qwe', find_game = 'False', score = 0, money = 0):	
	connection = connect()
	with connection:
		with connection.cursor() as cursor:
			i = 0
			query = f"SELECT * FROM users WHERE login='{login}'"
			cursor.execute(query)
			for row in cursor:
				i = 1
			if i == 0:
				query = f"""insert into users (login, password, find_game, score, money, queue, textures) 
					select '{login}', '{password}', '{find_game}', '{score}', '{money}', '{login}', '1' 
					from dual where not exists (select 1 from users where login = '{login}')"""
			
				cursor.execute(query)
				connection.commit()
				user_param = {'login': login, 'password': password, 'find_game': 'False', 'score': 0, 'money': 0, 'textures': '1'}
				return user_param
			else:
				return False

def find_user_by_key(key = 'find_game', value = 'True'):
	connection = connect()
	with connection:
		with connection.cursor() as cursor:
			query = f"SELECT * FROM users WHERE {key}='{value}'"
			cursor.execute(query)
			users_list = []
			for row in cursor:
				users_list.append(row)
			return users_list


def check_user(login, password):
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				query = f"SELECT * FROM users WHERE login='{login}' AND password='{password}'"
				cursor.execute(query)
				for row in cursor:
					user_param = row
				return user_param
	except:
		return False


def get_users_rating():
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				query = f"SELECT * FROM users WHERE score>0 ORDER BY score DESC"
				cursor.execute(query)
				users_list = []
				for row in cursor:
					users_list.append(row)
				return users_list
	except:
		return False


def get_textures():
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				query = f"SELECT * FROM shop"
				cursor.execute(query)
				texture_list = []
				for row in cursor:
					texture_list.append(row)
				return texture_list
	except:
		return False


def find_rooms():
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				query = f"SELECT * FROM rooms WHERE user2 IS NULL"
				cursor.execute(query)
				rooms = []
				for row in cursor:
					rooms.append(row)
				return rooms
	except:
		return False


def update_db_param(query):
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				cursor.execute(query)
				connection.commit()
	except:
		print(query + " - FAILED")
		return False


def insert_into_rooms(user1, status):
	connection = connect()
	with connection:
		with connection.cursor() as cursor:
			try:
				id_s = str(randint(100, 999))
				query = f"insert into rooms (id, user1, status) select '{id_s}', '{user1}', '{status}'"
				cursor.execute(query)
				connection.commit()
			except:
				id_s = str(randint(100, 999))
				query = f"insert into rooms (id, user1, status) select '{id_s}', '{user1}', '{status}'"
				cursor.execute(query)
				connection.commit()
			return id_s
			

def check_opponent(id_s):
	try:
		connection = connect()
		with connection:
			with connection.cursor() as cursor:
				while True:
					query = f"SELECT * FROM rooms WHERE id='{id_s}'"
					cursor.execute(query)
					for row in cursor:
						room_param = row
					if room_param["user2"] == None:
						return "None"
					else:
						return "True"
	except:
		return False

if __name__ == '__main__':
	print(create_user())
	print(find_user_by_key())