// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#define PORT 10018
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
        char resp[128] = "That is OK.\n";
	char *hello = "Hello from server";
	struct pollfd pfds[2];
        int timeout = 0;
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	pfds[0].fd = 0;
	pfds[0].events = POLLIN;


	pfds[1].fd = new_socket;
	pfds[1].events = POLLIN;

	while(1)
	{
                int ret = poll(pfds,2,100);
		if(ret > 0)
		{
                    timeout = 0;
                    if(pfds[1].revents & POLLIN){
                            bzero(buffer,sizeof(buffer));
                            valread = recv( new_socket , buffer, sizeof(buffer), 0);
                            if(valread > 0){
                                    printf("%s s:%d %d\n",buffer, valread, ret);
                                    send(new_socket,resp,strlen(resp),0);
                            }
                            else
                                    printf("status: %d\n",valread);
                    }
                    else
                            printf("no..\n");
		}
		else
		{
                    printf("ret: %d timeout: %d\n",ret,timeout);
                    timeout ++;
                    if(timeout > 50){
                        close(server_fd);
                        close(new_socket);
                        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
                        {
                                perror("socket failed");
                                exit(EXIT_FAILURE);
                        }

                        // Forcefully attaching socket to the port 8080
                        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                        {
                                perror("setsockopt");
                                exit(EXIT_FAILURE);
                        }
                        address.sin_family = AF_INET;
                        address.sin_addr.s_addr = INADDR_ANY;
                        address.sin_port = htons( PORT );

                        // Forcefully attaching socket to the port 8080
                        if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
                        {
                                perror("bind failed");
                                exit(EXIT_FAILURE);
                        }
                        if (listen(server_fd, 3) < 0)
                        {
                                perror("listen");
                                exit(EXIT_FAILURE);
                        }
                        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                                        (socklen_t*)&addrlen))<0)
                        {
                                perror("accept");
                                exit(EXIT_FAILURE);
                        }

                        pfds[0].fd = 0;
                        pfds[0].events = POLLIN;


                        pfds[1].fd = new_socket;
                        pfds[1].events = POLLIN;

                    }
		}
		
		//send(new_socket , hello , strlen(hello) , 0 );
		//printf("Hello message sent\n");
	}
	return 0;
}

