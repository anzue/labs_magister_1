import hashlib
import random

import mysql.connector

mydb = None


def connect_db():
    global mydb
    try:
        mydb = mysql.connector.connect(
            host="localhost",
            user="main_server",
            passwd="my_server_hard_password",
            database="mainschema",
            auth_plugin='mysql_native_password'
        )
    except Exception as ex:
        print(ex)
        return True
    print(mydb)
    print("Connection OK")
    return False


def add_user(username, password):
    if mydb is None:
        return False

    mycursor = mydb.cursor()
    mycursor.execute("SELECT * FROM Users where username='" + username + "'")
    myresult = mycursor.fetchall()
    print(myresult)
    if len(myresult) > 0:
        return False

    sql = "INSERT INTO Users (username, password_hash) VALUES (%s, %s)"
    hashed = int(hashlib.sha256(password.encode("utf-8")).hexdigest(), 16) % 2 ** 32
    print(username, hashed)
    val = (username, hashed)
    mycursor.execute(sql, val)

    mycursor.execute("SELECT * FROM Users where username='" + username + "'")
    myresult = mycursor.fetchall()
    for x in myresult:
        print(x)
    mydb.commit()
    return True


def get_hash(password):
    return int(hashlib.sha256(password.encode("utf-8")).hexdigest(), 16) % 2 ** 32


def check_user(session_id):
    mycursor = mydb.cursor()
    mycursor.execute("SELECT username FROM Users where session_id=" + session_id + "")
    myresult = mycursor.fetchall()
    print(myresult)
    print(session_id)
    if len(myresult) != 1:
        return False
    return myresult[0][0]


def login_user(username, password_hash):
    mycursor = mydb.cursor()
    mycursor.execute("SELECT password_hash,user_id FROM Users where username='" + username + "'")
    myresult = mycursor.fetchall()
    print(myresult)
    print(password_hash)
    if len(myresult) != 1:
        return False
    if myresult[0][0] != int(password_hash):
        return False

    sessios_id = random.randint(0, 2 ** 30)
    mycursor.execute("update Users set session_id = " + str(sessios_id) + " where user_id = " + str(myresult[0][1]))
    mydb.commit()
    return sessios_id


def get_user_id_by_sess_id(session_id):
    mycursor = mydb.cursor()
    mycursor.execute("SELECT user_id FROM Users where session_id='" + str(session_id) + "'")
    myresult = mycursor.fetchall()
    if len(myresult) != 1:
        return False
    return myresult[0][0]


def get_history(position, session_id):
    user_id = get_user_id_by_sess_id(session_id)
    if not user_id:
        return False

    mycursor = mydb.cursor()
    print(user_id)
    mycursor.execute(
        "SELECT input,translation,record_id FROM History where user_id=" + str(user_id) + " order by record_id desc")
    myresult = mycursor.fetchall()
    print(myresult)
    if len(myresult) <= position:
        return False

    return {'input': myresult[position][0], 'translation': myresult[position][1]}


def add_history(input, translation, session_id):
    user_id = get_user_id_by_sess_id(session_id)
    if not user_id:
        return
    if len(input) >= 255:
        input = input[:255]
    if len(translation) >= 255:
        translation = translation[:255]
    mycursor = mydb.cursor()
    mycursor.execute(
        "insert into History (input,translation,user_id) values ('" + input + "','" + translation + "'," + str(user_id) + ")")
    mydb.commit()


connect_db()
# mycursor = mydb.cursor()
# mycursor.execute("SELECT * FROM Users")
# myresult = mycursor.fetchall()
# print(login_user('username',get_hash("password")))
