import requests
import json
from kivy.app import App
from kivy.uix.screenmanager import Screen, ScreenManager

from MainScreen import MainScreen


class MyApp(App):
    def build(self):
        self.title = 'Text Style Transfer'
        sm = ScreenManager()
        screen = MainScreen(name="text screen")
        sm.switch_to(screen)
        return sm


if __name__ == '__main__':
    MyApp().run()
