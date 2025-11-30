// C++ program to show the example of server application in
// socket programming
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>


using namespace std;
class Server{
    private:
        sockaddr_in serverAddres;
        int serverSocket;
        WSADATA wsaData;
    public:
        Server(int port){
            WSAStartup(MAKEWORD(2,2), &wsaData);
            memset(&serverAddres, 0, sizeof(serverAddres));
            this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            this->serverAddres.sin_family = AF_INET;
            this->serverAddres.sin_port = htons(port);

        }

        void run(){
            bind(this->serverSocket,(struct sockaddr*)&serverAddres, sizeof(serverAddres));
            listen(serverSocket,5);
            int connectionClient = accept(this->serverSocket,nullptr,nullptr);
           
            char recvBuffer[1024] = {0};
            recv(connectionClient,recvBuffer,sizeof(recvBuffer),0);

            cout << "Messagem from Client: " << recvBuffer << endl;
            closesocket(connectionClient);
            closesocket(serverSocket);
            WSACleanup();
        }


};

int main(){
    Server server = Server(8080);
    server.run();
    return 0;
}
