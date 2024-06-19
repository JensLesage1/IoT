import pymysql

conn = pymysql.connect(
    host='sql7.freesqldatabase.com',
    database='sql7708327',
    user='sql7708327',
    password='uGxFBVsmRn',
    charset='utf8mb4',
    cursorclass=pymysql.cursors.DictCursor)


cursor = conn.cursor()
sql_query = """CREATE TABLE book (
    id integer PRIMARY KEY,
    author text NOT NULL,
    language text NOT NULL,
    title text NOT NULL
)"""
cursor.execute(sql_query)
conn.close()