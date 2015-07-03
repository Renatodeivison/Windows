

#include "VSMExample.h"

#ifdef WIN32
#include <conio.h>
#else
#include <sys/types.h> 
#include <sys/time.h> 
#include <unistd.h> 
#include <termios.h>
#include <socket.h>




int getch() 
{
    struct termios oldt,

    newt;
    int ch;

    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int kbhit( void ) 
{ 
    int sel_ret_val; 
    int s_set = STDIN_FILENO + 1; 


    fd_set  keyboard; 
    struct timeval timeout; 


    timeout.tv_sec = 0;  /* return immediately */ 
    timeout.tv_usec = 0; 


    FD_ZERO( &keyboard ); 
    FD_SET( STDIN_FILENO, &keyboard ); 


    if( (sel_ret_val = select(s_set, &keyboard, NULL, NULL, &timeout)) < 0 ) 
         return -1;  /* let caller deal with an error, or return 0 
                      * if you don't care. 
                      */ 


    if( FD_ISSET(STDIN_FILENO, &keyboard) ) 
         return 1;     /* there is something to read from the keyboard */ 


    return 0;   /* nothing to read */ 



} 
#endif


using namespace VSMEXAMPLE;


#ifndef WIN32
/**
* Make all charcters in the string uppercase,
* function does not exist for the gcc compiler.
*/
char* strupr(char* str) {
	int numberOfCharacters = (int)strlen(str);
	for(int i = 0; i < numberOfCharacters; i++) {
		str[i] = toupper(str[i]);
	}
	return str;
}
#else
	// This handle console window close down on Win32
	static CVSMExample *gVSMpointer = 0;

	void VSMExitHandler()
	{
		if(gVSMpointer)
		{
			gVSMpointer->Disconnect();
			delete gVSMpointer;
			gVSMpointer = 0;
		}
	}
#endif

/**
* @brief main.cpp: Main file for VSMEXAMPLE VSM.
* @include main.cpp 
*/


#define BUFFER_LENG 2041 

int main(int argc, char* argv[])
{	
	unsigned short port = 56000;
	char* addres = "localhost";
	char buf[BUFFER_LENG];
	int l = 0;


	CSocket socket_win(SOCK_DGRAM, IPPROTO_UDP, false);

	if(socket_win.BindLocal(addres, port) == SOCKET_ERROR)
		printf("Failed to bind");
	printf("Bind sucesfull");


	while(true){
		fflush(stdout);
		memset(buf,'\0', BUFFER_LENG);
		printf("Running");
		printf(" %b \n", socket_win.Receive( buf, l));
		Sleep(1000);
	}

	getch();
	/*
	try{
		bool Break=false;
		//default multicast address and ports
		char optionsFile[256] = "C++ VSMExample.xml";
		bool recordingEnabled = false;
		char recordingFile[256] = "recording.dli";
		bool replayEnabled = false;
		char replayFile[256] = "recording.dli";
		

		// Register Private Message
		CSerializedMessage::Register();

		// Create a new VSM object
		CVSMExample *pVSM=0;
		CVSMOptions *options = 0;
		try {			
			cout <<"Loading options from file \"" << optionsFile <<"\"" <<endl;
			options = new CVSMOptions(optionsFile);
			pVSM=new CVSMExample(options);
		} catch(CSDKException ex) {
			printf("Exception occured during VSM creation, %s\n", ex.GetMsg().c_str());
			delete pVSM;
			return -1;
		}
		catch(...) {
			cout <<"Unknown exception during VSM creation" <<endl;
			return -1;
		}

#ifdef WIN32
		// Handle the exit gracefully
		gVSMpointer = pVSM;
		atexit(VSMExitHandler);
#endif
		
		bool enableBroadcast = false;
		bool setBroadcastAddresses = true;
		char* broadcastEnabled = 0;
		if(enableBroadcast) {
			if(setBroadcastAddresses) {
				broadcastEnabled = (char *)"yes";
			}
			else {
				broadcastEnabled = (char *)"direct";
			}
		} else {
			broadcastEnabled = (char *)"no";
		}
		
		if (recordingEnabled)
		{
			pVSM->SetRecordingFile(recordingFile);
		}

		if (replayEnabled)
		{
			pVSM->SetReplayFile(replayFile);
		}

		//To disable CUCS:es using UDP Multcast comment the following line
		if(!pVSM->Connect(options->SendMulticastAddress, options->SendMulticastPort, options->ReceiveMulticastAddress, options->ReceiveMulticastPort)){
			printf("Unable to connect VSM to network!");
			return 0;
		}
		//To enable CUCS:es using UDP Broadcast switch the above with the following line
		//if(!pVSM->Connect("255.255.255.255", options->SendMulticastPort+2, "255.255.255.255", options->ReceiveMulticastPort+2)) return 0;
	
		printf("\nWelcome to the VSM Example from UAS Europe\n");
		printf("For configuration, goto: %s \n", pVSM->GetOptions()->RemoteDisplayURL.c_str());
#ifdef  WIN32 		
		printf("< To quit this application press 'q' key>\n\n");
#else
		printf("< To quit this application press 'q' followed by ENTER>\n\n");
#endif
		printf("Waiting for incoming CUCS connections...\n");
				
		while(!Break){
			if(kbhit()){
				int key=getch();
				switch(key){
					case 27: //Esc
					case 'Q':
					case 'q':
						Break = true;
						break;
					default:
						break;
				}
			}
			
			CBaseDLIMessage::CrossSleep(1000);			
		}
#ifdef WIN32
		if (gVSMpointer)
		{
			pVSM->Disconnect();
			delete pVSM;
			pVSM = 0;
			gVSMpointer = 0;
		}
#else
		pVSM->Disconnect();
		delete pVSM;
#endif
	}
	catch(CSDKException e){
		cerr << "Error: " << e.GetMsg() << endl;
		return -1;
	}*/
	return 0;
}
