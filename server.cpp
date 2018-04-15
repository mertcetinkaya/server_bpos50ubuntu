#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 #include<iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include <string>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#define BUFLEN 512  //Max length of buffer
#define PORT 9999   //The port on which to listen for incoming data
using namespace std;
void die(char *s)
{
    perror(s);
   exit(1);
}
 
int main(int argc, char *argv[])
{
    struct sockaddr_in si_me, si_other;
     
    int s, i;
    socklen_t slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
	exit(1);
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        perror("bind");
	exit(1);
    }
     
    //keep listening for data
    while(1)
    {	
        cout<<"Waiting for data...";
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            perror("recvfrom()");
	    exit(1);
        }
         
        //print details of the client/peer and the data received
        cout<<"Received packet from"<< inet_ntoa(si_other.sin_addr)<<":"<<ntohs(si_other.sin_port)<<"\n";
        cout<<"Data:"<< buf<< "\n";
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            perror("sendto()");
	    exit(1);
        }
close(s);
return 0;
    }
 
    close(s);
    return 0;
}
