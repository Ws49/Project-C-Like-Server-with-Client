
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
    string message;
    char recvBuffer[1024] = {0};
    
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
        this->message = "";
    
    }

    void run(){
        int Connection = connect(this->sock, (struct sockaddr*)&server, sizeof(this->server));

        if(Connection == SOCKET_ERROR){
            cout<<"Erro ao coenctar" << endl;
            exit(1);
        }
        cout<<"Conectado com Sucesso!!!\n";

        string recvMessage;
        while(recvMessage != "exit" and this->message != "exit"){
            cout<<"You Say: ";
            getline(cin, this->message);
            send(this->sock, this->message.c_str(), this->message.size(), 0);

            //fazendo papel do servidor
            recvMessage = string(recvBuffer);
            recv(this->sock,recvBuffer,sizeof(recvBuffer),0);    
            cout << "Server say: " << recvBuffer << endl;
        }

        //Fechar a conexao
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
    fflush(stdin);

    Client client = Client(ip, port);
    client.run();
    
    return 0;
}


