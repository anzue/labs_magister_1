import json

import requests

ngrok_port = 'http://0c8adda8.ngrok.io:80'
ip_port_local = "http://127.0.0.1:5000/translate/"
ip_port = "http://217.66.98.149:5000/translate"


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
        return json.loads(json.dumps({"result": "Server is unreachable, try again later"}))


def get_flask_translation(text: str, lang_from='eng', lang_to='deu', sess_id='', code=None):
    req = json.dumps({
        "text": text,
        "lang_from": lang_from,
        "lang_to": lang_to,
        "sess_id": sess_id})
    res = request(req, '/translate/')

    if res['result'] != 'OK':
        return res['result']

    return res['text']


def get_flask_sess_id(login: str, password: str):
    s = json.dumps({
        "login": login,
        "password": password})

    res = request(s, '/login/')
    return res
