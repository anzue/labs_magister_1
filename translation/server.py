#!flask/bin/python
from imports import *

from main import prepeare_model
from model import Model
import dbconnection

app = Flask(__name__)

translation_model = None


@app.route('/translate/', methods=['GET'])
def translate():
    print(request.json)

    if not request.json:
        abort(400)

    inp = json.loads(request.json)

    if 'text' not in inp or 'session_id' not in inp:
        abort(400)

    username = dbconnection.check_user(inp['session_id'])
    if not username:
        return jsonify({'result': 'Invalid session'}), 404

    print(inp)
    text = inp['text']
    #  text = "".join(list(chr(ord(c) + 1) for c in text))

    splited = re.split('[;\t\n.]', text)
    splited = [x for x in splited if len(x) > 5]
    print(splited)
    res = "\n".join(translation_model.test(x)[0] for x in splited)
    dbconnection.add_history(inp['text'], res, inp['session_id'])
    return jsonify({'text': res,
                    'result': 'OK'}), 201


@app.route('/history/', methods=['GET'])
def get_history():
    print(request.json)
    if not request.json:
        abort(400)

    json_req = json.loads(request.json)
    print(json_req)
    if not 'position' in json_req or not 'session_id' in json_req:
        abort(400)

    result = dbconnection.get_history(int(json_req['position']), int(json_req['session_id']))
    if not result:
        abort(400)

    result['result'] = 'OK'
    return result


@app.route('/login/', methods=['GET'])
def login():
    print(request.json)
    if not request.json:
        abort(400)

    json_req = json.loads(request.json)
    print(json_req)

    if 'login' not in json_req or 'password' not in json_req:
        abort(400)

    result = dbconnection.login_user(json_req['login'], json_req['password'])
    if result is False:
        return jsonify({'result': 'Wrong id or password',
                        'session_id': '0'}), 404

    # TODO generate session key here
    return jsonify({'result': 'OK',
                    'session_id': str(result)}), 201


def run_server():
    global translation_model
    max_size = 50000
    translation_model = prepeare_model("data/eng-deu.txt", max_size)
    # translation_model = Model('data/eng-deu.txt', max_size)
    translation_model.load()

    host_def = '0.0.0.0'
    host_try = '217.66..98.149z'
    try:
        app.run(host=host_try, debug=False)
    except:
        app.run(host=host_def, debug=False)


if __name__ == "__main__":
    run_server()
