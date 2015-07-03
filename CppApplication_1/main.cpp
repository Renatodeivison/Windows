/* 
 * File:   main.cpp
 * Author: Station1
 *
 * Created on June 25, 2015, 12:45 PM
 */
#include <stdio.h>
#include <winsock2.h>
#include <string>

#define SERVER "192.168.1.96"  //ip address of udp server
#define BUFLEN 6  //Max length of buffer
#define SouPORT 56000 

#pragma comment(lib,"ws2_32.lib") //Winsock Library

using namespace std;

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int slen;
    char buf[BUFLEN];
    char *message;
    slen = sizeof(server);
    
    //Prepare the sockaddr_in structure
    memset((char *) &server, 0, sizeof(s));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER);
    server.sin_port = htons(SouPORT);
    
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    
    printf("Initialised.\n");
    
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    
    printf("Socket created.\n");
    
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
    
    //Send some data
    while(1){
        message = "Olahi";
        Sleep(1000);
        
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &server, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }  
    }
    
    puts(buf);
    WSACleanup();
    closesocket(s);
    
    return 0;
}