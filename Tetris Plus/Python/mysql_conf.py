import pymysql
from pymysql.cursors import DictCursor


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
				query = f"""insert into users (login, password, find_game, score, money, queue) 
					select '{login}', '{password}', '{find_game}', '{score}', '{money}', '{login}' 
					from dual where not exists (select 1 from users where login = '{login}')"""
			
				cursor.execute(query)
				connection.commit()
				user_param = {'login': login, 'password': password, 'find_game': 'False', 'score': 0, 'money': 0}
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

if __name__ == '__main__':
	print(create_user())
	print(find_user_by_key())