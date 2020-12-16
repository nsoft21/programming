from flask import Flask, request
import openpyxl
import os.path
import datetime

app = Flask(__name__)

pwd = os.path.abspath(os.path.dirname(__file__))
cache_size = 1000
cache = []

def to_file():
    global cache
    book = None
    excel_db = os.path.join(pwd, 'data.xlsx')
    
    if not os.path.exists(excel_db):
        book = openpyxl.Workbook()
        
        book.active['A1'] = 'N'
        book.active['B1'] = 'User ID'
        book.active['C1'] = 'Datetime'
        book.active['D1'] = 'Item'
        book.active['E1'] = 'Price'
        book.save(excel_db)
    else:
        book = openpyxl.open(excel_db)

    sheet = book.active
    cur_time = datetime.datetime.utcnow()
    max_row = len(sheet['A'])
    row = max_row + 1

    for dataset in cache:
        id = dataset['user_id']
        for product in dataset['check']:
            sheet[row][0].value = row - 1
            sheet[row][1].value = id
            sheet[row][2].value = cur_time
            sheet[row][3].value = product['item']
            sheet[row][4].value = product['price']
            row += 1

    book.save(excel_db)
    book.close()

@app.route('/', methods=['POST'])
def index():
    global cache
    cache += [request.json]

    if (len(cache) > cache_size):
        to_file()
        cache = []

    return 'OK'
