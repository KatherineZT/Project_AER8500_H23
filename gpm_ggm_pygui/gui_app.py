import PySimpleGUI as sg
from socket_rw import *

# Window parameters
window_name = "AER8500 - User Interface"
margins = (50, 50)

# Window layout
layout = [
    [
        sg.Text('INPUTS', font=("Courier New", 14))
    ],
    [
        sg.Column([
            [sg.Text('Altitude')],
            [sg.In(size=(8, 1)), sg.Text('ft')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Climb Rate')],
            [sg.In(size=(8, 1)), sg.Text('m/min')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Angle of Attack')],
            [sg.In(size=(8, 1)), sg.Text('°')]
        ], element_justification='c')
    ],
    [
        sg.Text()
    ],
    [
        sg.Submit('SEND CMD'),
        sg.Cancel('CANCEL')
    ],
    [
        sg.Text()
    ],
    [
        sg.Text('STATE', font=("Courier New", 14))
    ],
    [
        sg.Column([
            [sg.Text('Altitude')],
            [sg.Text('300', size=(7, 1), text_color='gray', background_color='light gray'), sg.Text('ft')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Climb Rate')],
            [sg.Text('10', size=(7, 1), text_color='gray', background_color='light gray'), sg.Text('m/min')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Motor Power')],
            [sg.Text('30', size=(7, 1), text_color='gray', background_color='light gray'), sg.Text('%')]
        ], element_justification='c')
    ],
    [
        sg.Text()
    ],
    [
        sg.Text('CHANGEMENT_ALT', font=("Consolas", 13), background_color='green')
    ],
    [
        sg.Text()
    ],
    [
        sg.Text('Console:', text_color='black'),
        sg.Text('', size=(70, 1), text_color='red', background_color='black')
    ]
]

# Create window with parameters and layout above
window = sg.Window(title=window_name, layout=layout, margins=margins, element_justification='c').read()

# Create an event loop
while True:
    event, values = window.read()
    # End program if user closes window or
    # presses the OK button
    if event == "OK" or event == sg.WIN_CLOSED:
        break

window.close()