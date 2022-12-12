struct hostent;

int parseArguments(char* argument, char* user, char* pass, char* host, char* file_path);

void readServerResponse(int sockfd, char *response, char *fullResponse);

int login(int sockfd, char *user, char *pass);

int activatePassiveMode(int sockfd);

int download_file(int sockfd, int sockfd_client, char* file_path);
