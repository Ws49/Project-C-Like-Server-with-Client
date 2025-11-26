//cliente 
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


#define TAMANHO_BUFFER 1024
#define PORT "4444"
#define IP_SERVER "127.0.0.1"
/*
\\\\\\\\                                                      ////////
\                                                                    /
\                                                                    /
\                          DJ GABRIEL, O BRABO!                       /
\                                                                    /
\                                                                    /  
\\\\\\\\                                                      ////////

*/

int main (){
    WSADATA wsaData;
    int statusConnection;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL;
    struct addrinfo hints;
    char hostname[NI_MAXHOST];


    //buffers que serao usados
    char buffEnvio[TAMANHO_BUFFER];
    char buffRec[TAMANHO_BUFFER];

    
    statusConnection = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(statusConnection != 0){
        printf("WSAStartup falhou, erro: %d\n", statusConnection);
        system("pause");
        return 1;
    }

    //pegando info da maquina
    gethostname(hostname,NI_MAXHOST);//guardando o nome da maquina na variavel hostname
    printf("\nCliente: Nome: %s", hostname);
    PHOSTENT phe = gethostbyname(hostname);
    
    for (int i = 0; phe->h_addr_list[i] != 0; ++i){
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Cliente: IP: %d: %s\n", i,inet_ntoa(addr));
    }

    ZeroMemory(&hints,sizeof(hints));   
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    statusConnection = getaddrinfo(IP_SERVER,PORT,&hints,&result);
    if(statusConnection != 0){
        printf("getaddring falhou, erro: %d\n", statusConnection);
        WSACleanup();
        system("pause");
        return 1;
    }
        
    
    //tentando conectar ao servidor

    printf("\nTentando conectar no servidor, endereco %s, porta %s\n", IP_SERVER, PORT);
    
    //for ate se conectar
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next){
        //criar o socket
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(ConnectSocket == INVALID_SOCKET){
            printf("Socket falhou, erro: %d\n", WSAGetLastError());
            WSACleanup();
            system("pause");
            return 1;
        }
        //conectando    
        statusConnection = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if(statusConnection == SOCKET_ERROR){
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if(ConnectSocket == INVALID_SOCKET){
        printf("Nao foi possivel se conectar ao servidor!");
        system("pause");
        WSACleanup();
        return 1;
    }else{
        //MONTANDO MENSAGEM E ENVIANDO
        char * mensagem = (char*)malloc(1024 * sizeof(char));
        fgets(mensagem, 1024, stdin);

        printf("Eviando indentificacao");
        sprintf(buffEnvio, "Cliente %s\n", hostname);\
        statusConnection = send(ConnectSocket, buffEnvio, (int)strlen(buffEnvio), 0);

        
        sprintf(buffEnvio, "%s\n", mensagem);
        statusConnection = send(ConnectSocket, buffEnvio, (int)strlen(buffEnvio), 0);

        //testando se a mensagem foi enviada
        if(statusConnection = SOCKET_ERROR){
            printf("Falhou no envio de mensagem, erro: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            system("pause");
            return 1;
        }
        free(mensagem);
    }

    //encerrando conexao
    statusConnection = shutdown(ConnectSocket, SD_SEND);
    if(statusConnection == SOCKET_ERROR){
        printf("Falhou em encerrar a conexao %d\n", WSAGetLastError);
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
        return 1;
    }

    printf("Fechando Socket");
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}