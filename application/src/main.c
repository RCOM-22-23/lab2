#include "headers.h"

int main(int argc, char** argv){
	int	sockfd, sockfd_client;
	struct sockaddr_in server_addr;
	struct sockaddr_in server_addr_client;
    struct timeval init_time;
	struct timeval current_time;
    int port;

	char serverResponse[RESPONSE_SIZE];
	char fullResponse[BUFFER_SIZE];

	char user[MAX_SIZE];
	char pass[MAX_SIZE];
	char host[MAX_SIZE];
	char file_path[MAX_SIZE];

	struct hostent *h;

    //Check Input Size
	if (argc != 2) {
		fprintf(stderr,"Usage: ./download ftp://[user]:[pass]@[host]/[url-path]\n");
		exit(inputError);
	}

    //Parse input arguements
    if (parseArguments(argv[1], user, pass, host, file_path) < 0) {
		fprintf(stderr,"Usage: ./download ftp://[user]:[pass]@[host]/[url-path]\n");
		exit(parseError);
	}

    printf("\n-------------------- ARGUMENTS --------------------\n");
    printf("User: %s\n", user);
    printf("Pass: %s\n", pass);    
    printf("Host: %s\n", host);    
    printf("File_path: %s\n", file_path);

    //Get ip from domain name
    if ((h = gethostbyname(host)) == NULL) {
		fprintf(stderr,"Couldn't get Host IP\n");
		exit(domainNameError);
	}

    printf("\nHost name  : %s\n", h->h_name);
	printf("IP Address : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));
	printf("----------------------------------------------------\n");
	

    /*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr)));	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(SERVER_PORT);		/*server TCP port must be network byte ordered */

    /*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket()");
		exit(socketOpeningError);
	}

    /*connect to the server*/
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("connect()");
		exit(socketConnectingError);
	}

    printf("\n** Attempting connection to the server **\n");

    readServerResponse(sockfd, serverResponse, fullResponse);

    if (strncmp(serverResponse, "220", RESPONSE_SIZE) != 0) {
		fprintf(stderr,"Connection lost\n");
		exit(serverConnectionError);
	}

	if (login(sockfd, user, pass) < 0) {
		fprintf(stderr,"Couldn't login\n");
		exit(serverLoginError);
	}

	printf("\n** Attempting to enter passive mode **\n");

	if ((port = activatePassiveMode(sockfd)) < 0) {
		fprintf(stderr,"Couldn't enter passive mode\n");
		exit(passiveModeError);
	}

	printf("** Attempting connection to the client port **\n");

    /*server address handling*/
	bzero((char*)&server_addr_client,sizeof(server_addr_client));
	server_addr_client.sin_family = AF_INET;
	server_addr_client.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr)));	/*32 bit Internet address network byte ordered*/
	server_addr_client.sin_port = htons(port);		/*server TCP port must be network byte ordered */
    
	/*open an TCP socket*/
	if ((sockfd_client = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket()");
		exit(socketOpeningError);
	}

	/*connect to the server*/
	if(connect(sockfd_client, (struct sockaddr *)&server_addr_client, sizeof(server_addr_client)) < 0) {
		perror("connect()");
		exit(socketConnectingError);
	}

	printf("** Client connection successful **\n");

    printf("\n** Starting File Download **\n");


	gettimeofday(&init_time, 0);

	if (download_file(sockfd, sockfd_client, file_path) < 0) {
		fprintf(stderr,"Couldn't download file\n");
		exit(downloadingError);
	}

	gettimeofday(&current_time, 0);
	double elapsedTime = (current_time.tv_usec - init_time.tv_usec) / 1000.0 +
					(current_time.tv_sec - init_time.tv_sec) * 1000.0;

	char *filename = basename(file_path);

	printf("File %s downloaded successfully in %f seconds!\n", filename, elapsedTime/1000.0);

	if (close(sockfd_client) < 0) {
		perror("Error closing client socket");
		exit(socketClosingError);
	}

	if (close(sockfd)) {
		perror("Error closing server socket");
		exit(socketClosingError);
	}

	
    exit(0);
}