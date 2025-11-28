import socket

PORT = input("Digite a porta desejada: ")


sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.settimeout(None)
sock.bind(('localhost', int(PORT)))
sock.listen(1) 
print("---------Aguardando conexao---------")

connection, address = sock.accept()
print("Conectado em ", address)

while True:
    mensagem = connection.recv(1024)
    if not mensagem :
        connection.close()
        break
    print(mensagem.decode())
    connection.send(input("Digite  : ").encode())
    

sock.close()