import tkinter as tk
from tkinter import font

n = int(input())
class FullScreenApp:
    def __init__(self, master):
        self.master = master
        # 设置全屏显示
        master.attributes('-fullscreen', True)
        # 设置黑色背景
        master.configure(background='black')
        # 隐藏鼠标光标
        master.config(cursor='none')
        
        # 初始化数字
        self.counter = n
        
        # 创建自定义字体
        self.customFont = font.Font(family='Arial', size=200, weight='bold')
        
        # 创建数字标签
        self.label = tk.Label(master, text=str(self.counter), 
                            font=self.customFont, 
                            fg='white', 
                            bg='black')
        # 居中显示
        self.label.place(relx=0.5, rely=0.5, anchor='center')
        
        # 绑定退出快捷键（ESC键）
        master.bind('<Escape>', self.exit_app)

        self.auto_update()
        
    def auto_update(self):
        while(1):
            f=open('number.txt','r')
            tmp=f.read()
            self.counter=int(tmp)
            self.label.config(text=str(self.counter))
            f.close()
    
    def exit_app(self, event):
        self.master.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = FullScreenApp(root)
    root.mainloop()
