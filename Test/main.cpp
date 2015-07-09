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
#define SouPORT 14550

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
    mavlink_local_position_ned_t local_position_ned;
    mavlink_att_pos_mocap_t att_pos_mocap;
    mavlink_distance_sensor_t distance_sensor;
    mavlink_global_position_int_t global_position_int;
    
    
    closesocket(s);
    
    
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
    int sair = 0;
    while(msg.msgid != 33){
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
            if (mavlink_frame_char(MAVLINK_COMM_0, buf[i], &msg, &status) != MAVLINK_FRAMING_INCOMPLETE)
            {
                printf("Received message with ID %d, sequence: %d from component %d of system %d", msg.msgid, msg.seq, msg.compid, msg.sysid);
                if (msg.msgid == 33)
                {
                    mavlink_msg_global_position_int_decode(&msg, &global_position_int);
                    cout << endl<< endl << "Altitude: " << global_position_int.alt << endl << "Relative altitude: " << global_position_int.relative_alt << endl<<endl;
                }
            }
        }
        
       
//        for (int i = 0; i < receive; ++i)
//        {
//            temp = buf[i];
//                        printf("%02x ", (unsigned char)temp);
//            if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
//            {
//                                printf("\nReceived packet: SYS: %u, COMP: %u, LEN: %u, MSG ID: %u", msg.sysid, msg.compid, msg.len, msg.msgid);
//                                cout << " Payload: " << msg.payload64 << " \n\n";
//                mavlink_msg_attitude_encode(1, 200, &msg, &attitude);
//                                cout << "Yaw: " << attitude.yaw << " Pitch:" << attitude.pitch << "Roll: " << attitude.roll << endl;
//                mavlink_msg_local_position_ned_decode( &msg, &local_position_ned);
//                cout <<" Position z: "<< local_position_ned.z << endl << "Position x: "<< local_position_ned.x << endl <<
//                        "Position y: " << local_position_ned.y << endl << endl;
//                               mavlink_msg_att_pos_mocap_decode(&msg, &att_pos_mocap);
//                               cout << cout <<" Position z: "<< att_pos_mocap.z << endl << "Position x: "<< att_pos_mocap.x << endl <<
//                                       "Position y: " << att_pos_mocap.y << endl << endl;
//                               mavlink_msg_distance_sensor_decode(&msg, &distance_sensor);
//                               cout << "Distance: " << distance_sensor.current_distance << endl<< endl;
//                
//                if (msg.sysid == 33)   
//                {
//                    mavlink_msg_global_position_int_decode(&msg, &global_position_int);
//                    cout << "Altitude: " << global_position_int.alt << endl << "Relative altitude: " << global_position_int.relative_alt << endl<<endl;
//                }
//            }
//            
//        }
//        mavlink_msg_global_position_int_decode(&msg, &global_position_int);
//        cout << "Altitude: " << global_position_int.alt << endl << "Relative altitude: " << global_position_int.relative_alt << endl<<endl;
//        cin >> sair;
        sair++;
            //if(sair == 10) break;

    }
    
    puts(buf);
    WSACleanup();
    closesocket(s);
    return 0;
}

