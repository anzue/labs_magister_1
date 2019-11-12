import json

def get_flask_translation(text: str, lang_from='eng', lang_to='deu', sess_id='', code=None):
    s = json.dumps([{
        "text": text,
        "lang_from": lang_from,
        "lang_to": lang_to,
        "sess_id": sess_id}])
    print(s)
    try:
        res = requests.get("http://127.0.0.1:5000/translate/", json=s).json()  # 217.66.98.149 #
    except:
        return "Server is unreachable, try again later"
    print(res)
    return res['text']
