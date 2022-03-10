#!/usr/bin/env python
import PySimpleGUI as sg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import serial
import time
import threading
from multiprocessing import Process, Value, Array
from random import randint
import csv
import serial.tools.list_ports
QT_LOGGING_RULES="qt5ct.debug=false"


sg.theme("DarkTeal2")
#import pyautogui //to get the Position of the mouse
run = True
ser_error = False
data = ["0.0","0.0"]
def get_data(port):
    '''test the Serial connection and getting data from it
    split the data in a array with the first data to be x and
    the seccond to be y. saves the data in the global variable data'''
    global run, data, ser_error
    ser = serial.Serial(port, baudrate = 115200)
        
#     except:
#         print("Serial Error")
#         ser_error = True
#         exit() #Stops Thread, when a Serial Error accures
    #i = 0
    while run  == True:
        #time.sleep(0.05)
        #i+=1
        #data = [i, random.randint(0,10)]
        dat = str(ser.readline())[2:-5]
        data = dat.split(',')
    ser.close()

def draw_figure(canvas, figure):
    figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
    figure_canvas_agg.draw()
    figure_canvas_agg.get_tk_widget().pack(side='top', fill='both', expand=1)
    return figure_canvas_agg

def output(x,y):
    '''takes two arrays of datapoints (same length)
    and cretes a list with bothe values space by tabbing as a str
    to paste in a text-window'''

    outp = []
    for i in range(len(x)):
        outp.append(f"{str(x[i])}, \t \t{str(y[i])}")
    return outp

def csv_out(x,y,folder):
    '''get x and y list and a folderpath(str)
        creates a csv-file at the folder location'''
    csv_o = []
    with open(f'{folder}/tables.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        csv_o = list(zip(x,y))
        for row in csv_o:
            writer.writerow(row)
def list_serial():
    comlist = serial.tools.list_ports.comports()
    connected = []
    for element in comlist:
        connected.append(element.device)
    return connected      
# def list_com_test():
#     connected = [f'COM{randint(0,5)}', f'COM{randint(0,25)}',f'COM{randint(0,15)}',f'COM{randint(0,5)}',]
#     return connected

def gui():
    # global variables are neccessary because they are used in the main thread and the sup-thread
    # starts a thread, wich is continuesly updating the data comming from the serial port
    # important because so no delay will accure while running the gui
    
    global run, data, ser_error

    # starts the serial port selector window
    select_layout = [
    [sg.Text('Choose Serial Port', size=(30, 1))],
    [sg.OptionMenu((list_serial()), key = '-COMS-')],
    [sg.Button('OK'), sg.Button('Exit')]]
    
    # wait for choosing a serial port
    select_window = sg.Window('COM-Port wählen', select_layout, default_element_size=(40, 1), grab_anywhere=False, finalize = True)
    event, values = select_window.read()
    
    # if window is closed and no port choosen, then the programm will stop, else Serial port ist set to selectet com-Port 
    if event in ('Exit', None):
        select_window.close()
        return
    else:
        select_window.close()
    port = values['-COMS-']
    

    
   
    t1 = threading.Thread(target = get_data, args = (port,))
    t1.start()
    



    # define the form layout
    layout = [#[sg.Text('Titel', size=(40, 1), justification='center', font='Helvetica 20')],
              [sg.Canvas(size=(640, 480), key='-CANVAS-'),sg.Listbox(values = [], key = '-LISTOUT-', size=(30, 25))],
              [sg.Text(size=(40,2), key='-OUTPUT-', font = [25]), sg.Button('Export csv', disabled = True), sg.InputText(key = "-FOLDERTEXT-"),sg.FolderBrowse(key = "-FOLDER-")],
              [sg.Button('Start/Stop', disabled = False), sg.Button('Löschen', disabled = False)],
              [sg.Text('Messintervall'), sg.Slider((1, 100),1,1,orientation="h",size=(20, 15),enable_events = True, key="SLIDER",)],
              [sg.Text('Anzahl Messungen'), sg.Slider((10, 1000),100,10,orientation="h",size=(20, 15),enable_events = True, key="SLIDER_CROP",)],
              #[sg.Button("Serielle Verbindung starten", disabled = True)]
              #[sg.Button('Exit', size=(10, 2), pad=((280, 0), 3), font='Helvetica 14')]
               ]

    # create the form and show it without the plot
    window = sg.Window('Serial Plotter', layout, finalize=True)

    canvas_elem = window['-CANVAS-']
    canvas = canvas_elem.TKCanvas
    # draw the intitial scatter plot
    fig, ax = plt.subplots()
    ax.grid(True)
    fig_agg = draw_figure(canvas, fig)
    x = []
    y = []
    i = 0
    #n = 5
    pause = True
    x_offset = 0
    while True:

        #print(pyautogui.position())
        event, values = window.read(timeout=1)

        #if event == "Serielle Verbindung starten":
        #    t1 = threading.Thread(target = get_data, args = ())
        #    t1.start()

        try:
            datx = round(float(data[0])-x_offset,2)
            daty = float(data[1])
        except:
            datx = 0.0
            daty = 0.0

        if ser_error:
            window['-OUTPUT-'].update("Keine serielle Verbindung. Prüfen Sie die Verbindung mit dem Computer und starten Sie danach das Programm neu.")
            window.FindElement('Start/Stop').update(disabled = True)
            window.FindElement('Löschen').update(disabled = True)
            #window.FindElement('Serielle Verbindung starten').update(disabled = False)

        else:
            window['-OUTPUT-'].update(f"t = {datx}\n y = {daty }")



        if event in ('Exit', None):
            pause  = True
            run  = False
            t1.join()
            break



        crop = int(values['SLIDER_CROP'])
        x = x[-crop:]
        y = y[-crop:]

        if event == "-FOLDER-":
            window.FindElement('-FOLDERTEXT-').update(values["-FOLDER-"])

        folder = values["-FOLDER-"]
        #if there is no folder chosem, the export Button will be disabled

        if folder != '':
            window.FindElement('Export csv').update(disabled = False)

        # when clicking the export button a csv-file with the list of values will be createt in the chosen folder
        if event == 'Export csv':
            csv_out(x,y,folder)

        if event == 'Start/Stop':
            if pause  == False:
                pause  = True
                window.FindElement('Löschen').update(disabled = False)
            else:
                pause  = False
                window.FindElement('Löschen').update(disabled = True)

        if event == 'Löschen':
            datx = float(data[0])
            x_offset = datx
            x = []
            y = []
            ax.cla()
            ax.grid(True)
            ax.plot(x,y)
            fig_agg.draw()

         #updating values of x and y and plotting graph
        if pause  == False:
            n = values['SLIDER']
            i = int(datx*10)
            if i % n==0:
                y.append(float(daty))
                x.append(float(datx))
                ax.cla()
                ax.grid(True)
                ax.plot(x,y)
                fig_agg.draw()
                if n>=2:
                    time.sleep(0.05)
                   
           

            #takes the x and y values of the array and pushes them to the list-output
            window.FindElement('-LISTOUT-').Update(values=output(x,y),scroll_to_index=len(x))

        #Actions when plotting is paused
        elif pause == True:
            if values['-LISTOUT-'] ==[]: # checks if the list-output is empty
                pass
            else: # if the list-output is not empty, you can mark points, thet will be shown in the diagramm
                mark = values['-LISTOUT-'][0]
                markxy = mark.split(',')
                ax.cla()
                ax.grid(True)
                ax.plot(x,y)
                ax.plot(float(markxy[0]),float(markxy[1]),'ro')
                fig_agg.draw()


    window.close()



if __name__ == '__main__':
    gui()
