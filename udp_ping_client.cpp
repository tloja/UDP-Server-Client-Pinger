//UDP Pinger

// Must have a UDP server running before you run this UDP Pinger Client

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctime>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include <errno.h>

// defines are PORT number
#define PORT	12000

int main(){
	int sockfd, n;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr;

	//creating our ping and our ping number identifier	
	char* ping = (char*)"Ping";
	int ping_num;

	//creating the UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	//set up memory block for server address and client address
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	//set up server information
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY; //this can be changed to a specific address for host-to-host pinging
	servaddr.sin_port = htons(PORT); 
	//for loop to send the 10 pings
	for(int i = 0; i < 10; i++){	
	
		//Defining our Timeout which will be 1 second.
		struct timeval timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

		//Setup error checking for if the setsockopt fails
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
			printf("the setsockopt function failed\n");
		};
		
		//defining our start time using chrono
		auto start_time = std::chrono::system_clock::now();	
		
		//sending ping to socket, with ping message, no flags
		sendto(sockfd, (const char *)ping, strlen(ping), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
		
		//reading message from server, providing socket descriptor, buffer, and length of buffer
		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, (struct sockaddr *) &servaddr, &len); 

		//adding 1 to our i and printing the correct ping number
		ping_num = i+1;
		printf("Ping #%d\n", ping_num);
		
		//defining our end time using chrono
		auto end_time = std::chrono::system_clock::now();

		//calculating our RTT into microseconds
		int RTT = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
				
	       	//if the recvfrom does not receive packet
	       	if(n==-1){
			printf("The packet was lost.\n");
		}

		// successful path
		else {
			printf("Success! RTT For this trip: %i microseconds.\n",RTT);
		}

		// if setsockopt function fails 
		if(setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
			printf("Time out!");
			close(sockfd);
		}
	}
	//close the socket
	close(sockfd) ; 
	return 0;
}
