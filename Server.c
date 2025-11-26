#include<winsock2.h>
#include<windows.h>
#include<ws2tcpip.h>
#include<stdlib.h>
#include<stdio.h>


#define TAMANHO_BUFFER 1024
#define PORT "4444"

/*
\\\\\\\\                                                      ////////
\                                                                    /
\                                                                    /
\                          DJ ISAQUE, O BRABO!                       /
\                                                                    /
\                                                                    /  
\\\\\\\\                                                      ////////

*/

int main(){
    WSADATA wsaData;
    int resultSock;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo * resultAddr = NULL;
    struct addrinfo hints;

    int checkSend;
    char recvBuffer[TAMANHO_BUFFER];
    int recvLenBuffer = TAMANHO_BUFFER;

    resultSock = WSAStartup(MAKEWORD(2,2), &wsaData);

    

    if(resultSock != 0){
        printf("WSAStartup falhou com erro: %d\n",resultSock);
        system("pause");
        return 1;
    }

    char hostname[NI_MAXHOST];
    gethostname(hostname,NI_MAXHOST);

    system("cls || clear");
    printf("--> SERVER NAME : %s\n",hostname);

    PHOSTENT myHost = gethostbyname(hostname);
    for(int i =0;myHost->h_addr_list[i]; ++i){
        struct in_addr addr;
        memcpy(&addr,myHost->h_addr_list[i],sizeof(struct in_addr));
        printf("--> Servidor: my addres IP %d : %s\n",i,inet_ntoa(addr));
    }

    ZeroMemory(&hints,sizeof(hints));   
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    resultSock = getaddrinfo(NULL,PORT,&hints,&resultAddr);

    if(resultSock != 0){
        printf("getaddrinfo falhou com erro: %d\n",resultSock);
        WSACleanup();
        system("pause");
        return 1;
    }

    listenSocket = socket(resultAddr->ai_family, resultAddr->ai_socktype,resultAddr->ai_protocol);
    if(listenSocket == INVALID_SOCKET){
        printf("socket falhou com erro: %ld\n",WSAGetLastError());  
        freeaddrinfo(resultAddr);
        WSACleanup();
        system("pause");
        return 1;
    }

    resultSock = bind(listenSocket,resultAddr->ai_addr,(int)resultAddr->ai_addrlen);
    if(resultSock == SOCKET_ERROR){
        printf("bind falhou com erro: %d\n",WSAGetLastError());
        freeaddrinfo(resultAddr);
        closesocket(listenSocket);
        WSACleanup();
    }

    freeaddrinfo(resultAddr);

    printf("--> Servidor: lisetining PORT[%s]...\n",PORT);
    resultSock = listen(listenSocket,SOMAXCONN);

    if(resultSock == SOCKET_ERROR){
        printf("erro ao ouvir portas do server: %d\n",WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        system("pause");
        return 1;        
    }

    ClientSocket = accept(listenSocket,NULL,NULL);
    if(ClientSocket == INVALID_SOCKET){
        printf("erro ao aceitar a conexção: %d\n",WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        system("pause");
        return 1;  
    }

    closesocket(listenSocket);


    do{
        resultSock = recv(ClientSocket,recvBuffer,recvLenBuffer,0);
        if(resultSock > 0){
            printf("--> Client say : %s \n",recvBuffer);
        }else if(resultSock == 0){
            puts("ERRO AO CONECTAR");
        }else{
            printf("erro ao receber dados : %d\n",WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            system("pause");
            return 1;  
        }
    }while(resultSock > 0);

    resultSock = shutdown(ClientSocket,SD_SEND);
    if(resultSock == SOCKET_ERROR){
        printf("erro ao desligar conexao : %d\n",WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        system("pause");
        return 1;  
    }

    printf("Quit....");
    closesocket(ClientSocket);
    WSACleanup();
    system("pause");
    return 0;
}