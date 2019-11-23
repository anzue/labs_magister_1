import requests
from kivy.app import App
from kivy.input.providers.mouse import Color
from kivy.properties import NumericProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.dropdown import DropDown
from kivy.uix.label import Label
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.uix.textinput import TextInput
from kivy.graphics import Color, Rectangle

from flask import get_flask_translation, get_history


class MainScreen(Screen):
    def __init__(self, **kwargs):
        super(MainScreen, self).__init__(**kwargs)
        self.session_id = 0
        self.lang_from = 'English'
        self.lang_to = 'English'
        self.history_position = 0

        def on_enter(instance, value):
            print('User pressed enter in', instance)

        def on_text(instance, value):
            print('The widget', instance, 'have:', value)

        def on_focus(instance, value):
            if value:
                print('User focused', instance)
            else:
                print('User defocused', instance)

        textinput = TextInput(font_size='20sp')
        textinput.bind(focus=on_focus)
        textinput.bind(text=on_text)
        textinput.bind(on_text_validate=on_enter)
        label = Label(
            text='Translation will be here',
            size_hint=(1, 1),
            font_size='20sp',
            color=(1, 1, 1, 1),
            halign="left",
            valign="top",
            #    text_size= (self.width, self.height)
        )

        with label.canvas.before:
            Color(1, 1, 1, 1)
            Rectangle(pos=label.pos, size=label.size)

        layout = BoxLayout(orientation='horizontal')
        layout.add_widget(textinput)
        layout.add_widget(label)

        button_layout = BoxLayout(orientation='horizontal', size_hint=(1, 0.2))

        def shift_hist_position(instance, shift):
            self.history_position += shift
            if self.history_position < 0:
                self.history_position = 0
            if self.history_position > 100:
                self.history_position = 100
            res = get_history(self.history_position, self.session_id)
            print(res)
            if res['result'] != 'OK':
                return
            self.textinput.text = res['input']
            self.recoginzed.text = res['translation']
            print(self.history_position)

        def translate(instance):
            print('The button <%s> is being pressed' % instance.text)
            self.recoginzed.text = "Waiting for translation"
            self.recoginzed.text = get_flask_translation(self.textinput.text, session_id=self.session_id)

        def clear_text(instance):
            self.recoginzed.text = "";
            self.textinput.text = ""

        button_prev = Button(text='Previous', font_size=14)
        button_prev.bind(on_press=lambda x: shift_hist_position(x, 1))
        button_layout.add_widget(button_prev)

        button_next = Button(text='Next', font_size=14)
        button_next.bind(on_press=lambda x: shift_hist_position(x, -1))
        button_layout.add_widget(button_next)

        button_translate = Button(text='Request translation', font_size=14)
        button_translate.bind(on_press=translate)
        button_layout.add_widget(button_translate)

        button_clear = Button(text='Clear', font_size=14)
        button_clear.bind(on_press=clear_text)
        button_layout.add_widget(button_clear)
        # languages = BoxLayout(orientation='horizontal')
        # lang_from = BoxLayout()
        # lang_to = BoxLayout()
        #
        # self.supported_languages = ['English', 'Spanish']
        # for x in self.supported_languages:
        #     btn = Button(text=x, size_hint_y=None, height=40)
        #     def on_release(button):
        #         lang_from.select(button)
        #         self.lang_from = x
        #     btn.bind(on_release=lambda btn: on_release(btn))
        #     lang_from.add_widget(btn)
        #     btn2 = Button(text=x, size_hint_y=None, height=40)
        #     def on_release_to(button):
        #         lang_to.select(button)
        #         self.lang_to = x
        #     btn2.bind(on_release=lambda btn2: on_release_to(btn2))
        #     lang_to.add_widget(btn2)
        #
        # languages.add_widget(lang_from)
        # languages.add_widget(lang_to)

        total_layout = BoxLayout(orientation='vertical')
        #  total_layout.add_widget(languages)
        total_layout.add_widget(button_layout)
        total_layout.add_widget(layout)

        self.textinput = textinput
        self.recoginzed = label
        self.button_translate = button_translate
        self.add_widget(total_layout)
