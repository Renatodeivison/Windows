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
#define SouPORT 14551 

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
    unsigned int temp = 0;
    char buf[BUFLEN];
    slen = sizeof(server);
    ssize_t receive;
    uint8_t buffer;
    
    
    mavlink_message_t msg;
    mavlink_status_t status;
    mavlink_global_position_int_t global_position_int;
    mavlink_vicon_position_estimate_t vicon_position_estimate;
    mavlink_vfr_hud_t vfr_hud;
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
    
//    global_position_int.alt = 50000;
//    global_position_int.vx = 4;
//    global_position_int.vy = 0;
//    global_position_int.vz = 0;
//    mavlink_msg_global_position_int_encode(1, 33, &msg, &global_position_int);
//    mavlink_msg_to_send_buffer((uint8_t *)&buf, &msg);
//    
//        // Send an initial buffer
//    send( s, buf, (int)strlen(buf), 0 );

    int sair = 0;
    int a = 0;
    int b,c,d;
    while(true){
        memset(buf,'\0', BUFLEN);
        
        //try to receive some data, this is a blocking call
        receive = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server, &slen);
        if (receive == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        
        //cout << "Bytes Received: " << (int)receive << "\nDatagram:\n";
        
        for (int i = 0; i < receive; ++i)
        {
            if (mavlink_frame_char(MAVLINK_COMM_0, buf[i], &msg, &status) == MAVLINK_FRAMING_INCOMPLETE)
            {
                //printf("Received message with ID %d, sequence: %d from component %d of system %d\n\n", msg.msgid, msg.seq, msg.compid, msg.sysid);
                switch (msg.msgid)
                {
                    case 33:
                        mavlink_msg_global_position_int_decode(&msg, &global_position_int);
                        cout << endl<< endl << "Altitude: " << global_position_int.alt/1000 << endl << "Relative altitude: " << global_position_int.relative_alt/1000 <<endl<<endl;
                        b = 1;
                        a = b + c + d;
                        break;
                    case 30:
                        mavlink_msg_attitude_decode(&msg, &attitude);
                        cout << endl<< endl << "Roll angle (rad): " << attitude.roll*57.2957795131 << endl << "Pitch angle (rad): " << attitude.pitch*57.2957795131 << endl << "Yaw angle (rad): "<< attitude.yaw*57.2957795131 <<endl<<endl;
                        c = 1;
                        a = b + c + d;
                        break;
                    case 74:
                        mavlink_msg_vfr_hud_decode( &msg, &vfr_hud);
                        cout << endl<< endl << "Airspeed(m/s): " << vfr_hud.airspeed << endl << "Ground speed(m/s): " << vfr_hud.groundspeed <<  endl << endl;
                        d = 1;
                        a = b + c + d;
                        break;
                        
                }
            }
//            else if(mavlink_frame_char(MAVLINK_COMM_0, buf[i], &msg, &status) == MAVLINK_FRAMING_INCOMPLETE)
//            {
//                cout <<endl<<endl<< "***Mavlink Frame incomplete.***"<<endl<<endl;
//            }
//            else
//            {
//                cout <<endl<<endl<< "***Mavlink Frame Bad CRC.***"<<endl<<endl;
//            }
        }
        Sleep(1000);
     sair++; if (a == 3) break;    
    }      
    
    WSACleanup();
    closesocket(s);
    return 0;
}

