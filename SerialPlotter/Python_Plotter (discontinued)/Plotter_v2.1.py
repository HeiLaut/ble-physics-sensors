# TODO: Übergabe des Messintervalls über serielle Verbinung und damit Steuerung

#!/usr/bin/env python
import PySimpleGUI as sg
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import serial
#import time
import threading
#from multiprocessing import Process, Value, Array
#from random import randint
import csv
import serial.tools.list_ports
QT_LOGGING_RULES="qt5ct.debug=false"


sg.theme("DarkTeal2")
#import pyautogui //to get the Position of the mouse
run = True
ser_error = False
data = ["0.0","0.0"]
serialData = [[],[],"x","y"] # datx, daty, labelx, labely

def get_data(port):
    '''test the Serial connection and getting data from it
    split the data in a array with the first data to be x and
    the seccond to be y. saves the data in the global variable data'''
    global run, data, ser_error
    ser = serial.Serial(port, baudrate = 115200)

    while run:
        #print(ser.readline())
        dat = str(ser.readline())[2:-5]
        data = dat.split(',')
        
        # versuche die daten immer zu lesen und insgesamt abzurufen, unabhängig vom fenster!
        # alldata speichert 2 Listen. besser liste von tupeln nutzen, bzw. zwei arrays damit
        # die daten direkt im Diagramm abgetragen werden können
        try:
            serialData[2] = data[0]
            serialData[3] = data[2]
            serialData[0].append(float(data[1]))
            serialData[1].append(float(data[3]))
        except:
            pass
        #print(len(serialData[0]))
            
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
    # safety, if the lists are not exactly the same size. sometimes a few items in one list are missing, when there is fast measurements
    if len(x) > len(y):
        l = len(y)
    else:
        l = len(x)
    for i in range(l):
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

    valx = serialData[2]
    valy = serialData[3]
    
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
    
    pause = False


    # define the form layout
    layout = [#[sg.Text('Titel', size=(40, 1), justification='center', font='Helvetica 20')],
              [sg.Canvas(size=(640, 480), key='-CANVAS-'),sg.Listbox(values = [], key = '-LISTOUT-', size=(30, 25))],
              [sg.Text(size=(40,2), key='-OUTPUT-', font = [25]), sg.Button('Export csv', disabled = True), sg.InputText(key = "-FOLDERTEXT-"),sg.FolderBrowse(key = "-FOLDER-")],
              [sg.Button('Start/Stop', disabled = False), sg.Button('Löschen', disabled = False)],
              [sg.Text('Messintervall'), sg.Slider((1, 100),1,1,orientation="h",size=(20, 15),enable_events = True, key="SLIDER",)],
              [sg.Text('Anzahl Messungen'), sg.Slider((10, 2000),100,10,orientation="h",size=(20, 15),enable_events = True, key="SLIDER_CROP",)],
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
    
#    x = serialData[0]
 #   y = serialData[1]
  #  i = 0
    #n = 5
    while True:

        #print(pyautogui.position())
        event, values = window.read(timeout=1)

        #if event == "Serielle Verbindung starten":
        #    t1 = threading.Thread(target = get_data, args = ())
        #    t1.start()

        #try:
        #    datx = round(float(data[0])-x_offset,2)
        #    daty = float(data[1])
        #except:
        #    datx = 0.0
        #    daty = 0.0

        if ser_error:
            window['-OUTPUT-'].update("Keine serielle Verbindung. Prüfen Sie die Verbindung mit dem Computer und starten Sie danach das Programm neu.")
            window.find_element('Start/Stop').update(disabled = True)
            window.find_element('Löschen').update(disabled = True)
            #window.find_element('Serielle Verbindung starten').update(disabled = False)

        else:
            # output data on screen every update
            if len(serialData[0])>1:
                window['-OUTPUT-'].update(f"{serialData[2]}: {serialData[0][-1]}\n{serialData[3]}: {serialData[1][-1] }")



        if event in ('Exit', None):
            pause  = True
            run  = False
            t1.join()
            break


        #crop data 
        crop = int(values['SLIDER_CROP'])
        serialData[0] = serialData[0][-crop:]
        serialData[1] = serialData[1][-crop:]
        
        # allways keep same length of both data, in case a value is lost in translation
        serialData[0] = serialData[0][0:len(serialData[1])]
        serialData[1] = serialData[1][0:len(serialData[0])]

        if event == "-FOLDER-":
            window.find_element('-FOLDERTEXT-').update(values["-FOLDER-"])

        folder = values["-FOLDER-"]
        #if there is no folder chosem, the export Button will be disabled

        if folder != '':
            window.find_element('Export csv').update(disabled = False)

        # when clicking the export button a csv-file with the list of values will be createt in the chosen folder
        if event == 'Export csv':
            csv_out(tempx,tempy,folder)

        if event == 'Start/Stop':
            if pause  == False:
                pause  = True
                window.find_element('Löschen').update(disabled = False)
                #window.find_element("SLIDER_CROP").update(disabled = True)

            else:
                pause  = False
                window.find_element('Löschen').update(disabled = True)
                #window.find_element("SLIDER_CROP").update(disabled = False)


        if event == 'Löschen':
           #datx = float(data[0])
            #x_offset = serialData[0][-1]
            serialData[0] = []
            serialData[1] = []
            tempx = []
            tempy = []
            ax.cla()
            ax.grid(True)
            ax.plot(serialData[0],serialData[1])
            fig_agg.draw()

         #updating values of x and y and plotting graph
        if pause  == False:
            ax.cla()
            ax.grid(True)
            #print(len(serialData[0]),len(serialData[1]))
            ax.plot(serialData[0],serialData[1])
            
            #store data for processing them while pause (mark points)
            tempx = serialData[0]
            tempy = serialData[1]
            
            fig_agg.draw()
#             n = values['SLIDER']
#             i = int(datx*10)
#             if i % n==0:
#                 #y.append(float(daty))
#                 #x.append(float(datx))
#                
#                 if n>=2:
#                     time.sleep(0.05)
#                    
           

            #takes the x and y values of the array and pushes them to the list-output
            window.find_element('-LISTOUT-').Update(values=output(serialData[0],serialData[1]),scroll_to_index=len(serialData[0]))

        #Actions when plotting is paused
        elif pause == True:
            if values['-LISTOUT-'] ==[]: # checks if the list-output is empty
                pass
            else: # if the list-output is not empty, you can mark points, thet will be shown in the diagramm
                mark = values['-LISTOUT-'][0]
                markxy = mark.split(',')
                ax.cla()
                ax.grid(True)
                ax.plot(tempx,tempy)
                ax.plot(float(markxy[0]),float(markxy[1]),'ro')
                fig_agg.draw()


    window.close()



if __name__ == '__main__':
    gui()
