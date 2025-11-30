
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unistd.h>



using namespace std;


class Client{
    private:
    int port;
    string ip;
    SOCKET sock;
    WSADATA wsa;
    sockaddr_in server;
    
    public:
    Client(string ip, int port){
        this->ip = ip;
        this->port = port;

        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
            cout << "Erro ao inicializar WSA!\n";
            exit(1);
        }

        this->sock = socket(AF_INET, SOCK_STREAM, 0);
        if(this->sock == INVALID_SOCKET){
            cout<<"Erro ao criar o socket"<< endl;
            exit(1);
        }

        this->server.sin_family = AF_INET;
        this->server.sin_port = htons(port);
        this->server.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    void run(){
        int result = connect(this->sock, (struct sockaddr*)&server, sizeof(this->server));
        if(result == SOCKET_ERROR){
            cout<<"Erro ao coenctar" << endl;
            exit(1);
        }

        cout<<"Conectado com Sucesso!!!";
        string message = "olaaaa, conexao estabelecida!!!";
        send(this->sock, message.c_str(), message.size(), 0);
        closesocket(this->sock);
        ZeroMemory(&server, sizeof(server));
        WSACleanup();

    }
};

int main(){
    string ip;
    int port;
    cout<<"Digite o ip e a porta: "<<endl;
    cin>>ip;
    cin>>port;

    Client client = Client(ip, port);
    client.run();
    
    return 0;
}


