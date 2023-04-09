import PySimpleGUI as sg
from socket_rw import *
import time as time
import _thread

def error_message():
    sg.Popup("Error! Check your values...", keep_on_top=True)

'''def update_value(window):
    current_time = int(round(time.time() * 100)) - start_time
    window[0].update('{:02d}:{:02d}.{:02d}'.format((current_time // 100) // 60,
                                                                  (current_time // 100) % 60,
                                                                  current_time % 100))
    return 1'''

# Window parameters
window_name = "AER8500 - User Interface"
margins = (50, 50)

# Out parameters
target_altitude = 0
target_climbing_speed = 0
angle = 0

# In parameters
altitude = 0
climbing_speed = 0
motor_power = 0

start_time = 0

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
window = sg.Window(title=window_name, layout=layout, margins=margins, element_justification='c')
window.read(timeout=1)
window.find_element(0).Update(0)
window.find_element(1).Update(0)
window.find_element(2).Update(0)

# Init connection to calculator
init_socket()

current_time = 0
paused = False
start_time = int(round(time.time() * 100))

#_thread.start_new_thread( update_value, (window, ) )

# Create an event loop
while True:
    event, values = window.read(timeout=1000)
    # End program if user closes window or
    # presses the OK button

    if (event != "__TIMEOUT__"):
        tmp_altitude = int(values[0])
        tmp_climb_speed = float(values[1])
        tmp_angle = float(values[2])
        window.find_element(0).Update(0)
        window.find_element(1).Update(0)
        window.find_element(2).Update(0)
        error = False

        if (tmp_altitude != target_altitude and tmp_altitude <= 40000 and tmp_altitude >= 0 and \
            tmp_climb_speed == 0 and tmp_angle == 0):
            send_cmd(1, tmp_altitude)
            target_altitude = tmp_altitude

        elif(tmp_altitude == 0):
            if (tmp_climb_speed != target_climbing_speed and tmp_climb_speed <= 800 and tmp_climb_speed >= -800 and \
                ((tmp_climb_speed >= 0 and angle >= 0) or (tmp_climb_speed < 0 and angle < 0))):    
                send_cmd(2, tmp_climb_speed)
                tmp_climb_speed = target_climbing_speed
            else:
                error = True

            if (tmp_angle != angle and tmp_angle <= 16 and tmp_angle >= -16 and \
                ((tmp_angle >= 0 and target_climbing_speed >= 0) or (tmp_angle < 0 and target_climbing_speed < 0))):
                send_cmd(3, tmp_angle)
                tmp_angle = angle
            else:
                error = True
        else:
            error = True
        print("hi")
        if error : 
            error_message()

    new_data = recv_cmd()

    if event == "OK" or event == sg.WIN_CLOSED:
        break
    


window.close()