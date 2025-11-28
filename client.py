import socket
import customtkinter
import tkinter as tk
import threading

class ViewClient:
    def __init__(self):
        self.window = customtkinter.CTk()
        customtkinter.set_appearance_mode("dark")
        self.window.geometry("1280x720")
        self.window.title("DolphinsChat")
        #print(customtkinter.CTkInputDialog(title="Reposdta").get_input())
        self.labelName = customtkinter.CTkLabel(self.window,text="Chat Dolphins",font=customtkinter.CTkFont(family="Courier New",size=64),text_color="#a806ee")
        self.fieldChat = customtkinter.CTkFrame(self.window,width=1000,height=600,border_color="#a806ee",border_width=2)
        self.fieldUsersChat = customtkinter.CTkFrame(self.window,width=220,height=600,border_color="#a806ee",border_width=2)
        self.inputMessage = customtkinter.CTkEntry(self.fieldChat,width=800,height=40,border_color="#a806ee")
        self.buttonSendMessage = customtkinter.CTkButton(self.fieldChat,width=120,height=40,border_color="#a806ee",border_width=2,fg_color="#333232",hover_color="#1D1C1C",text="Enviar")        
        self.filedMessageChat = customtkinter.CTkScrollableFrame(self.fieldChat,width=800,height=450,border_color="#a806ee",border_width=2)

        self.window.protocol("WM_DELETE_WINDOW", self.isClosing)
        
    def isClosing(self):
        self.controll.clientSocket.close()
        self.controll.IsClose = False
        self.window.destroy()
        
    def setControll(self, controll):
        self.controll = controll
        
    def draw(self):
        self.filedMessageChat.place(x=20,y=50)
        self.inputMessage.place(x=20,y=550)
        self.buttonSendMessage.place(x=850,y=550)   
             
        self.labelName.place(x=440,y=0)
        self.fieldChat.place(x=20,y=100)
        
        self.fieldUsersChat.place(x=1040,y=100)
        self.window.resizable(False,False)
        self.window.mainloop()
        
    def addMessage(self,message : str):
        newMessage = customtkinter.CTkLabel(self.filedMessageChat,text=message)
        newMessage.pack(anchor="w", pady=5)

    @property
    def button(self):
        return self.buttonSendMessage
    
    @property
    def entryMessage(self):
        return self.inputMessage
    
    @property
    def frameMessages(self):
        return self.frameMessages
    
class ControllChat:
    
    def __init__(self,view : ViewClient, hostname,port):
        self.view = view
        self.hostname = hostname
        self.port = port
        self.clientSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.view.button.configure(command=send_message)
        self.view.setControll(self)
        self.IsClose = True
        thread = threading.Thread(target=self.run)
        thread.start()

    
    def run(self):
        self.clientSocket.connect((self.hostname, int(self.port)))
        mensagem = b''
        try:
            while self.IsClose == True:
                mensagem = self.clientSocket.recv(1024)
                if not mensagem:
                    self.clientSocket.close()
                    break
                controll.view.addMessage(message=mensagem)
        except Exception as e:
            if not ("[WinError 10053]" in str(e)):
                print(f'Erro as : {e}')
              



def send_message(): 
    controll.clientSocket.send(controll.view.entryMessage.get().encode())
    controll.view.addMessage(controll.view.entryMessage.get())
    controll.view.entryMessage.delete(0,customtkinter.END)

if __name__ == "__main__":
    view = ViewClient()
    controll = ControllChat(view,customtkinter.CTkInputDialog(title="Informe o host : ",text="Informe o host : ").get_input(),customtkinter.CTkInputDialog(title="Informe a porta : ",text="Informe a porta : ").get_input())
    view.draw()
    

