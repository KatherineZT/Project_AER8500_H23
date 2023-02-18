import PySimpleGUI as sg

# Window parameters
window_name = "AER8500 - User Interface"
size=(500,300)

# Window layout
layout = [
    [sg.Text("Hello from PySimpleGUI")], [sg.Button("OK")]]

# Create window with parameters and layout above
window = sg.Window(title=window_name, layout=layout, margins=size).read()

# Create an event loop
while True:
    event, values = window.read()
    # End program if user closes window or
    # presses the OK button
    if event == "OK" or event == sg.WIN_CLOSED:
        break

window.close()