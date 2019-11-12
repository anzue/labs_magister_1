from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.screenmanager import Screen
from kivy.uix.textinput import TextInput

from flask import get_flask_sess_id


class LoginScreen(Screen):
    def __init__(self, switch_main_callback, **kwargs):
        super(LoginScreen, self).__init__(**kwargs)

        layout = BoxLayout(orientation='vertical')

        self.login = TextInput(text='Enter login')
        self.password = TextInput(text='Enter password')
        self.submit_btn = Button(text='Submit')

        def on_press(inst):
            res = get_flask_sess_id(self.login.text, self.password.text)
            print("res = ", res)
            if res['result'] == 'OK':
                print("Login ok")
                switch_main_callback()
                return

        self.submit_btn.bind(on_press=on_press)  # Note: here say_hello doesn't have brackets.

        layout.add_widget(self.login)
        layout.add_widget(self.password)
        layout.add_widget(self.submit_btn)

        self.add_widget(layout)
