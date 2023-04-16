import PySimpleGUI as sg
from socket_rw import *


# Window parameters
window_name = "AER8500 - User Interface"
margins = (50, 50)

# Out parameters
target_altitude = 0
target_climbing_speed = 0
angle = 0

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
            [sg.Text('0', size=(7, 1), text_color='gray', background_color='light gray', key = 'alt'), sg.Text('ft')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Climb Rate')],
            [sg.Text('0.0', size=(7, 1), text_color='gray', background_color='light gray', key = 'clb'), sg.Text('m/min')]
        ], element_justification='c'),
        sg.Column([
            [sg.Text('Motor Power')],
            [sg.Text('0', size=(7, 1), text_color='gray', background_color='light gray', key = 'mtr'), sg.Text('%')]
        ], element_justification='c')
    ],
    [
        sg.Text()
    ],
    [
        sg.Text('AU_SOL', font=("Consolas", 13), background_color='green', key = 'sta')
    ],
    [
        sg.Text()
    ],
    [
        sg.Text('Console:', text_color='black'),
        sg.Text('', size=(75, 1), text_color='red', background_color='black', key='cons')
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

chute_libre = False

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
        window.find_element('cons').Update("")

        if (tmp_altitude != target_altitude and tmp_altitude <= 40000 and tmp_altitude >= 0):
            if (tmp_climb_speed == 0 and tmp_angle == 0):
                send_cmd(1, tmp_altitude)
                target_altitude = tmp_altitude
            else:
                window.find_element('cons').Update("ERREUR: POUR NOUVELLE ALTITUDE, VITESSE ET ANGLE NE DOIVENT PAS ÊTRE ENTRÉS")
        else:
            window.find_element('cons').Update("ERREUR: ALTITUDE DOIT ÊTRE ENTRE 0 ET 40000 PIEDS")

        if(tmp_altitude == 0):
            if (tmp_climb_speed != target_climbing_speed and tmp_climb_speed <= 800 and tmp_climb_speed >= -800 and \
                ((tmp_climb_speed >= 0 and tmp_angle >= 0) or (tmp_climb_speed < 0 and tmp_angle < 0))):    
                if (tmp_angle != angle and tmp_angle <= 16 and tmp_angle >= -16):
                    if ((tmp_angle >= 0 and tmp_climb_speed >= 0) or (tmp_angle < 0 and tmp_climb_speed < 0)):
                        if (tmp_angle > 15 or tmp_angle < -15):
                            window.find_element('cons').Update("CHUTE LIBRE, REDRESSEZ L'AÉRONEF")
                            chute_libre = True
                        else:
                            window.find_element('cons').Update("")
                            chute_libre = False
                        send_cmd(2, tmp_climb_speed)
                        send_cmd(3, tmp_angle)
                        tmp_climb_speed = target_climbing_speed
                        tmp_angle = angle
                    else:
                        window.find_element('cons').Update("ERREUR: ANGLE D'ATTAQUE ET VITESSE DOIVENT AVOIR MÊME SIGNE")
                else:
                    window.find_element('cons').Update("ERREUR: ANGLE D'ATTAQUE DOIT ÊTRE ENTRE -16 ET +16 DEGRÉS")
            else:
                window.find_element('cons').Update("ERREUR: VITESSE DOIT ÊTRE ENTRE -800 ET +800 MÈTRES/MINUTE")

        if (tmp_climb_speed != target_climbing_speed and (tmp_altitude != 0 or tmp_angle == 0)):
            window.find_element('cons').Update("ERREUR: POUR NOUVELLE VITESSE, ANGLE DOIT ÊTRE ENTRÉ, MAIS PAS ALTITUDE")

        if (tmp_angle != angle and (tmp_altitude != 0 or tmp_climb_speed == 0)):
            window.find_element('cons').Update("ERREUR: POUR NOUVEL ANGLE, VITESSE DOIT ÊTRE ENTRÉE, MAIS PAS ALTITUDE")
            
    else :
        new_data = recv_cmd(window, chute_libre)

    if event == "OK" or event == sg.WIN_CLOSED:
        break

window.close()