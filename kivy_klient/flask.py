import hashlib
import json

import requests

ngrok_port = 'http://0c2377d6.ngrok.io:80'
ip_port_local = "http://127.0.0.1:5000"
ip_port = "http://217.66.98.149:5000"


def request(req, req_path):
    try:
        print("Request = ", req)
        print(ngrok_port + req_path)
        res_str = requests.get(ngrok_port + req_path, json=req, timeout=7).json()
        print(res_str)
        #  res = json.loads(res_str)
        #  print("Answer = ", res)
        return res_str
    except:
        try:
            res_str = requests.get(ip_port_local + req_path, json=req, timeout=7).json()
            return res_str
        except:
            print("error in request")
        return json.loads(json.dumps({"result": "Server is unreachable, try again later"}))


def get_flask_translation(text: str, lang_from='eng', lang_to='deu', session_id='', code=None):
    req = json.dumps({
        "text": text,
        "lang_from": lang_from,
        "lang_to": lang_to,
        "session_id": session_id})
    res = request(req, '/translate/')

    if res['result'] != 'OK':
        return res['result']

    return res['text']


def get_hash(password):
    return int(hashlib.sha256(password.encode("utf-8")).hexdigest(), 16) % 2 ** 32


def get_flask_session_id(login: str, password: str):
    s = json.dumps({
        "login": login,
        "password": str(get_hash(password))})

    res = request(s, '/login/')
    return res


def get_history(id: int, session_id: int):
    s = json.dumps({
        "position": id,
        "session_id": str(session_id)})

    res = request(s, '/history/')
    return res
