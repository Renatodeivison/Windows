/* 
 * File:   main.cpp
 * Author: Station1
 *
 * Created on July 3, 2015, 12:24 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream>

#include "C:\Users\Station1\Desktop\mavlink-master\out\common\mavlink.h"
#include "C:\Users\Station1\Desktop\mavlink-master\out\mavlink_helpers.h"
#include "C:\Users\Station1\Desktop\mavlink-master\out\protocol.h"
#include "C:\Users\Station1\Desktop\mavlink-master\out\mavlink_types.h"


#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 283  //Max length of buffer
#define SouPORT 56000

#pragma comment(lib,"ws2_32.lib") //Winsock Library

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int slen;
    char buf[BUFLEN];
    char *message;
    slen = sizeof(server);
    mavlink_message_t msg;
    const mavlink_message_t* msg1;
    mavlink_status_t status;
    unsigned int temp = 0;
    ssize_t receive;   
    mavlink_attitude_t attitude;
    
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
    printf("Bind done.\n");
    //Send some data
    while(1){
       // message = "Olahi";
        // Sleep(1000);
         
        // if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &server, slen) == SOCKET_ERROR)
         //{
        // printf("sendto() failed with error code : %d" , WSAGetLastError());
        // exit(EXIT_FAILURE);
        // }
        // puts(buf);
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        
        //try to receive some data, this is a blocking call
        receive = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server, &slen);
        if (receive == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        
        cout << "Bytes Received: " << (int)receive << "\nDatagram:\n";
        for (int i = 0; i < receive; ++i)
        {
            temp = buf[i];
//            printf("%02x ", (unsigned char)temp);
            if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
            {
//                printf("\nReceived packet: SYS: %u, COMP: %u, LEN: %u, MSG ID: %u", msg.sysid, msg.compid, msg.len, msg.msgid);
//                cout << " Payload: " << msg.payload64 << " \n\n";
                mavlink_msg_attitude_encode(1, 200, &msg, &attitude);
                cout << "Yaw: " << attitude.yaw << " Pitch:" << attitude.pitch << "Roll: " << attitude.roll << endl;
              
            }
        }
    }
    
    puts(buf);
    WSACleanup();
    closesocket(s);
    return 0;
}

