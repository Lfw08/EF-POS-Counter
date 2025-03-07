import tkinter as tk
from tkinter import font
n=0
class FullScreenApp:
    def __init__(self, master):
        self.master = master
        # 设置全屏显示
        master.attributes('-fullscreen', True)
        # 设置黑色背景
        master.configure(background='#000000')
        # 隐藏鼠标光标
        master.config(cursor='none')
        
        # 初始化数字
        self.counter = n
        
        # 创建自定义字体
        self.customFont = font.Font(family='Arial', size=500, weight='bold')
        
        # 创建数字标签
        self.label = tk.Label(master, text=str(self.counter), 
                            font=self.customFont, 
                            fg='#FFFFFF',
                            bg='#000000')
        # 居中显示
        self.label.place(relx=0.5, rely=0.5, anchor='center')

        # 绑定退出快捷键（ESC键）
        master.bind('<Escape>', self.exit_app)

        self.timing()
    
    def timing(self):
        print(1)
        self.auto_update()
        self.master.after(500,self.timing)

    def auto_update(self):
        f=open('/home/raspberrypi/EF-POS-Counter/number.txt','r')
        tmp=f.read()
        self.counter=int(tmp)
        self.label.config(text=str(self.counter))
        print(self.counter)
        f.close()
    
    def exit_app(self, event):
        self.master.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = FullScreenApp(root)
    root.mainloop()
