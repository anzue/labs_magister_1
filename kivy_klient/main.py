import requests
import json
from kivy.app import App
from kivy.uix.screenmanager import Screen, ScreenManager

from LoginScreen import LoginScreen
from MainScreen import MainScreen


class MyApp(App):
    def build(self):
        self.title = 'Text Translation'
        self.sm = ScreenManager()

        self.main_screen = MainScreen(name="main screen")

        def move_to_main():
            self.sm.switch_to(self.main_screen)

        self.login_screen = LoginScreen(switch_main_callback=move_to_main, name="Login screen")

        self.sm.switch_to(self.login_screen)

        return self.sm


if __name__ == '__main__':
    MyApp().run()
