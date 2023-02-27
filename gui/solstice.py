# (c) 2023 Open Risk, all rights reserved


import webbrowser

from kivy.app import App
from kivy.clock import Clock
from kivy.properties import NumericProperty
from kivy.uix.accordion import Accordion, AccordionItem
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.image import Image
from kivy.uix.label import Label
from kivy.uix.popup import Popup
from kivy.uix.togglebutton import ToggleButton
from ruamel.yaml import YAML


class AppConfig:
    def __init__(self):
        pass


class SolsticeApp(App):
    __version__ = '0.0.1'


def build(self):
    self.icon = 'Data/Solstice.png'
    root = Root()
    return root


# define the main widget as a BoxLayout
class Root(BoxLayout):
    in_yaml = YAML(typ='unsafe')
    inFile = open('configuration.yml', 'r')
    inFile.close()


    def increment_time(self, dt):
        self.time += dt

    def time_reset(self):
        self.time = 0

    # instantiation of the root widget
    def __init__(self, **kwargs):
        # print('init')
        super(Root, self).__init__(**kwargs)

        Clock.schedule_interval(self.increment_time, self.time_delta)
        Clock.schedule_once(self.show_gameover, self.duration)

        # initialize a portfolio
        pass

    # execute this when user clicks the restart button
    def reset(self):
        self.time = 0

        Clock.unschedule(self.show_gameover)
        Clock.schedule_once(self.show_gameover, self.duration)

    # execute this when the user wants to read the full documentation
    def web_docs(self):
        webbrowser.open_new("https://www.openriskmanagement.com/solstice/")

    # execute this when a different loan is selected
    def set_selected(self, selloan):
        pass

    # execute this when a different loan is selected
    def select_other(self, instance):
        pass

    # execute this when there is a request for new loan data
    def display_newloan(self, newloan):
        pass

    def accept(self):
        pass

    def reject(self):
        # initialize new loan data
        pass

    def update_color(self):
        pass

    #
    # execute this whenever there is a portfolio data refresh event
    #
    def display_portfolio(self, selected_id):
        pass

    #
    # function to show an About popup
    #
    def about_popup(self):
        pass

    #
    # function to show the instructions popup
    #
    def learn_popup(self):
        objective = """TODO"""
        portfolio = """TODO"""
        new_loans = """TODO"""
        metrics = """TOOD"""

        pass

    #
    # function to show a Game Over popup
    #
    def show_gameover(self, instance):
        pass

    #
    # cleaning up before shutting down
    #
    def onclose(self, instance):
        pass

    # function to Quit the App
    def exit(self):
        App.get_running_app().stop(self)


if __name__ == '__main__':
    SolsticeApp().run()
