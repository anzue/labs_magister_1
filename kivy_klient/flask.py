import json

import requests

def get_flask_translation(text: str, lang_from='eng', lang_to='deu', sess_id='', code=None):
    s = json.dumps([{
        "text": text,
        "lang_from": lang_from,
        "lang_to": lang_to,
        "sess_id": sess_id}])
    print(s)
    try:
        ngrok_port = 'http://cf97e4d.ngrok.io:80'
        ip_port_local = "http://127.0.0.1:5000/translate/"
        ip_port = "http://217.66.98.149:5000/translate"
        res = requests.get(ngrok_port, json=s, timeout=2).json()  # 217.66.98.149 #
    except:
        return "Server is unreachable, try again later"
    print(res)
    return res['text']
