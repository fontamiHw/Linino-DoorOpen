/*
 * server.cpp
 *
 *  Created on: May 22, 2014
 *      Author: mfontane
 */
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int sockfd, newsockfd, portno;
int alarmNum=0;

void errorServer(const char *msg)
{
	perror(msg);
	close(newsockfd);
	close(sockfd);
	exit(1);
}

void closeAll() {
	close(newsockfd);
	close(sockfd);
}

void server_thread(void *arg) {
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		errorServer("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi("5555");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0)
		errorServer("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,
			(struct sockaddr *) &cli_addr,
			&clilen);
	if (newsockfd < 0)
		errorServer("ERROR on accept");
	bzero(buffer,256);

	do {
		n = read(newsockfd,buffer,255);
		if (n < 0)
			errorServer("ERROR reading from socket");

		char command[255];
		struct timespec ts = { 0 };

		clock_gettime(CLOCK_REALTIME, &ts);
		//zip -m /opt/DoorAlarm/Alarm_%d /opt/DoorAlarm/*.jpg"
		sprintf(command, " tar cvf /opt/DoorAlarm/Alarm_%d.tar /opt/DoorAlarm/*.jpg && rm /opt/DoorAlarm/*.jpg", alarmNum++);
		system(command);

//		sprintf("sec: %ld nsec: %ld\n", ts.tv_sec, ts.tv_nsec);
		printf("Here is the message: %s\n",buffer);
		n = write(newsockfd,"I got your message",18);
		if (n < 0)
			errorServer("ERROR writing to socket");
	} while(1);
}
