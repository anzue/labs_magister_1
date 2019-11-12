#!flask/bin/python
from imports import *

from main import prepeare_model
from model import Model

app = Flask(__name__)

translation_model = None


@app.route('/translate/', methods=['GET'])
def translate():
    print(json)
    if not request.json or 'text' not in request.json:
        abort(400)
    inp = json.loads(request.json)
    print(inp)
    text = inp['text']
    #  text = "".join(list(chr(ord(c) + 1) for c in text))

    splited = re.split('[;\t\n.]', text)
    splited = [x for x in splited if len(x) > 5]
    print(splited)
    res = "\n".join(translation_model.test(x)[0] for x in splited)

    return jsonify({'text': res,
                    'result': 'OK'}), 201


login_password = {
    'admin': 'admin',
    'login': 'password'
}


@app.route('/login/', methods=['GET'])
def login():
    print(request.json)
    if not request.json:
        abort(400)

    json_req = json.loads(request.json)
    print(json_req)

    if 'login' not in json_req or 'password' not in json_req:
        abort(400)

    if json_req['login'] not in login_password or login_password[json_req['login']] != json_req['password']:
        return jsonify({'result': 'wrong id or password'}), 404

    # TODO generate session key here
    return jsonify({'result': 'OK',
                    'sess_id': '12345'}), 201


def run_server():
    global translation_model
    max_size = 50000
    translation_model = prepeare_model("data/eng-fra.txt", max_size)
    # translation_model = Model('data/eng-deu.txt', max_size)
    translation_model.load()
    host_def = '0.0.0.0'
    host_try = '217.66..98.149z'
    try:
        app.run(host=host_def, debug=False)
    except:
        app.run(host=host_def, debug=False)


if __name__ == "__main__":
    run_server()
