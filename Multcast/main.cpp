/* 
 * File:   main.cpp
 * Author: Station1
 *
 * Created on July 1, 2015, 11:22 AM
 */

#include <stdio.h>
#include <winsock2.h>
#include <string>

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 52001   //The port on which to listen for incoming data
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#ifndef IP_MULTICAST_IF 
/* 
 * The following constants are taken from include/netinet/in.h
 * in Berkeley Software Distribution version 4.4. Note that these 
 * values *DIFFER* from the original values defined by Steve Deering 
 * as described in "IP Multicast Extensions for 4.3BSD UNIX related 
 * systems (MULTICAST 1.2 Release)". It describes the extensions 
 * to BSD, SunOS and Ultrix to support multicasting, as specified
 * by RFC-1112. 
 */ 
#define IP_MULTICAST_IF 9 /* set/get IP multicast interface */ 
#define IP_MULTICAST_TTL 10 /* set/get IP multicast TTL */
#define IP_MULTICAST_LOOP 11 /* set/get IP multicast loopback */ 
#define IP_ADD_MEMBERSHIP 12 /* add (set) IP group membership */ 
#define IP_DROP_MEMBERSHIP 13 /* drop (set) IP group membership */ 
#define IP_DEFAULT_MULTICAST_TTL 1 
#define IP_DEFAULT_MULTICAST_LOOP 1 
#define IP_MAX_MEMBERSHIPS 20 
/* The structure used to add and drop multicast addresses */ 
typedef struct ip_mreq { 
  struct in_addr imr_multiaddr; /* multicast group to join */
  struct in_addr imr_interface; /* interface to join on */
}IP_MREQ; 
#endif 

#define DESTINATION_MCAST "234.5.6.7" 
#define DESTINATION_PORT 4567 
 
  int nRet, nSize, nOptVal; 
  SOCKET hSock; 
  struct sockaddr_in stSourceAddr, stDestAddr; 
  u_short nSourcePort; 
  struct ip_mreq stIpMreq; 
  /* get a datagram (UDP) socket */ 
  hSock = socket(PF_INET, SOCK_DGRAM, 0); 

  /*----------------------- to send ---------------------------

  /* Theoretically, you do not need any special preparation to 
   * send to a multicast address. However, you may want a few
   * things to overcome the limits of the default behavior 
   */

  /* init source address structure */ 
  stSourceAddr.sin_family = PF_INET; 
  stSourceAddr.sin_port = htons(nSourcePort); 
  stSourceAddr.sin_addr.s_addr = INADDR_ANY; 

  /* 
   * Calling bind() is not required, but some implementations need it 
   * before you can reference any multicast socket options
   */ 
  nRet = bind (hSock, 
         (struct sockaddr FAR *)&stSourceAddr, 
         sizeof(struct sockaddr)); 
  if (nRet == SOCKET_ERROR) { 
     
  } 

  /* disable loopback of multicast datagrams we send, since the 
   * default--according to Steve Deering--is to loopback all
   * datagrams sent on any interface which is a member of the
   * destination group address of that datagram.
   */ 
  nOptVal = FALSE; 
  nRet = setsockopt (hSock, IPPROTO_IP, IP_MULTICAST_LOOP, 
        (char FAR *)nOptVal, sizeof(int)); 
  if (nRet == SOCKET_ERROR) { 
    /* rather than notifying the user, we make note that this option 
     * failed. Some WinSocks don't support this option, and default
     * with loopback disabled), so this failure is of no consequence.
     * However, if we *do* get loop-backed data, we'll know why
     */ 
    bLoopFailed = TRUE; 
  } 

  /* increase the IP TTL from the default of one to 64, so our
   * multicast datagrams can get off of the local network 
   */
  nOptVal = 64; 
  nRet = setsockopt (hSock, IPPROTO_IP, IP_MULTICAST_TTL,
         (char FAR *)nOptVal, sizeof(int)); 
  if (nRet == SOCKET_ERROR) { 
     
  } 
 
  /* Initialize the Destination Address structure */ 
  stDestAddr.sin_family = PF_INET; 
  stDestAddr.sin_addr.s_addr = inet_addr (DESTINATION_MCAST);
  stDestAddr.sin_port = htons (DESTINATION_PORT); 
 
  nRet = sendto (hSock, (char FAR *)achOutBuf, 
          lstrlen(achOutBuf), 0, 
          (struct sockaddr FAR *) &stDestAddr, 
          sizeof(struct sockaddr)); 
  if (nRet == SOCKET_ERROR) { 
     
  } 
 
  /*----------------------- to receive -------------------------
   * Register for FD_READ events (any operation mode will work, but 
   * we happened to use asynchronous mode in this example 
   */
  nRet = WSAAsyncSelect (hSock, hwnd, WM_READ_DATA, FD_READ);

  if (nRet == SOCKET_ERROR) { 
     
  } 
 
  /* join the multicast group we want to receive datagrams from */ 
  stIpMreq.imr_multiaddr.s_addr = DESTINATION_MCAST; /* group addr */ 
  stIpMreq.imr_interface.s_addr = INADDR_ANY; /* use default */ 
  nRet = setsockopt (hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
          (char FAR *)&stIpMreq, sizeof (struct ip_mreq));

  if (nRet == SOCKET_ERROR) { 
     
  } 

  /* multicast datagram receive routine from our Window Procedure */ 
  case WM_READ_DATA: 
    if (WSAGETSELECTERROR (lParam)) { 
       
    } 
    switch (WSAGETSELECTEVENT (lParam)) { 
      case FD_READ: 
        /* Recv the available data */ 
        nSize = sizeof(struct sockaddr); 
        nRet = recvfrom (hSock, (char FAR *)achInBuf, 
                BUFSIZE, 0, 
                (struct sockaddr *) &stSockAddr, &nSize); 
        if (nRet == SOCKET_ERROR) { 
           
        } 
        break; 
    }