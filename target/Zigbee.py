import serial
import time
import tkinter as tk
import threading

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

s.write("+++")
char = s.read(2)
print("Enter AT mode.")
print(char)

s.write("ATMY 0x16\r\n")
char = s.read(3)
print("Set MY 0x16.")
print(char)

s.write("ATDL 0x18\r\n")
char = s.read(3)
print("Set DL 0x18.")
print(char)

s.write("ATWR\r\n")
char = s.read(3)
print("Write config.")
print(char)

s.write("ATMY\r\n")
char = s.read(3)
print("MY :")
print(char)

s.write("ATDL\r\n")
char = s.read(3)
print("DL : ")
print(char)

s.write("ATCN\r\n")
char = s.read(3)
print("Exit AT mode.")
print(char)

###############################################################################
# Tkinter
###############################################################################
window = tk.Tk()
window.title('my window')
window.geometry('800x500')

var = tk.StringVar()
l = tk.Label(window,textvariable=var, bg='white', font=('Arial', 30), width=60,
             height=10)
l.pack()

count_down = 60
var.set(count_down)

def update_timeText():
# Get the current time, note you can change the format as you wish
    global count_down 
    count_down = count_down-1

    var.set(count_down)
    # Call the update_timeText() function after 1 second
    window.after(1000, update_timeText)


def thread_job():
    if(s.read(2)):
        added_thread2._Thread__stop()
        while(1):
            var.set("hitting the target")

added_thread = threading.Thread( target=thread_job )
added_thread.start()
added_thread2 = threading.Thread( target=update_timeText )

on_hit = False
def hit_me():
    global on_hit
    if on_hit == False:
        on_hit = True
        s.write("start")
        added_thread2.start()
    else:
        on_hit = False
        #s.write("pause")

b = tk.Button(window, text='game start', width=15,
              height=2, command=hit_me)
b.pack()

count_down = 60
var.set("Are you ready?????")

window.mainloop()


s.close()
