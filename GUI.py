from tkinter import *
from tkinter import ttk

a="1145"

#read number from file
a = ''
with open('number.txt', 'r') as file:
    for line in file:
        a = line
        a = a.strip("\n")
        a = "数量：" + a 


root = Tk()
root.attributes('-fullscreen', True)


label01=Label(root, text=a, font=("Arial", 200),)
label01.pack(expand=True)


button1=Button(root, text="Quit", command=root.destroy)
button1.pack(side=BOTTOM,fill=X)

root.mainloop()

