#include "headers.h"
#include "ftp.h"

void readServerResponse(int sockfd, char *response, char *fullResponse) {
    code_state state = start;
    char character;
    int i = 0;

    while(state != code_received) {
        read(sockfd, &character, 1);

        fullResponse[i] = character;
        i++;

        switch(state) {
        case start:
            if (isdigit(character)) {
            state = first_digit;
            response[0] = character;
            }
            break;
        case first_digit:
            if (isdigit(character)) {
            state = second_digit;
            response[1] = character;
            }
            else {
            state = start;
            memset(response,0,sizeof(response));
            }
            break;
        case second_digit:
            if (isdigit(character)) {
            state = third_digit;
            response[2] = character;
            }
            else {
            state = start;
            memset(response,0,sizeof(response));
            }
            break;
        case third_digit:
            if ((character == ' ')) {
            state = last_line;
            }
            else {
            state = start;
            memset(response,0,sizeof(response));
            }
            break;
        case last_line:
            if ((character == '\n')) {
            state = code_received;
            }
            break;
        }
    }

    fullResponse[i] = '\0';
    printf("\nServer Response:\n%s\n", fullResponse);
}


int parseArguments(char* argument, char* user, char* pass, char* host, char* file_path) {

    // Parse the initial part of the argument: "ftp://"
    if(strncmp(argument, "ftp://", 6) != 0) {
        fprintf(stderr,"Invalid Argument: it should start with 'ftp://'\n");
            return -1;
    }

    int i = 6, j;

    if(strchr(argument, '@') != NULL) {   // User and pass present in input
        // Parse the part of the argument relative to the user
        while (argument[i] != ':') {
        user[i - 6] = argument[i];
        i++;
        }
        user[i - 6] = '\0';

        // Parse the part of the argument relative to the pass
        i++;
        j = i;
        while (argument[j] != '@') {
        pass[j - i] = argument[j];
        j++;
        }
        pass[j - i] = '\0';
        j++;
    }
    else {
        user[0] = '\0';
        pass[0] = '\0';
        j = 6;
    }

    // Parse the part of the argument relative to the host
    int k = j;
    while (argument[k] != '/') {
        host[k - j] = argument[k];
        k++;
    }
    host[k - j] = '\0';

    // Parse the part of the argument relative to the url path
    k++;
    int l = k;
    while (argument[l] != '\0') {
        file_path[l - k] = argument[l];
        l++;
    }
    file_path[l - k] = '\0';

    return 0;
}

int login(int sockfd, char *user, char *pass) {
    char localResponse[RESPONSE_SIZE];
    char localFullResponse[BUFFER_SIZE];
    localResponse[0] = '4';

    printf(">> Writing username to socket ...\n");

    while(localResponse[0] == '4') {
        write(sockfd, "user ", 5);
        write(sockfd, user, strlen(user));
        write(sockfd, "\n", 1);
        readServerResponse(sockfd, localResponse, localFullResponse);
    }

    if (localResponse[0] == '2') {   // Password not requested - Login successful
        return 0;
    }

    if(localResponse[0] != '3') {
        fprintf(stderr,"Username not accepted\n");
            return -1;
    }

    memset(localResponse,0,sizeof(localResponse));
    memset(localFullResponse,0,sizeof(localFullResponse));
    localResponse[0] = '4';

    printf(">> Writing password to socket ...\n");

    while(localResponse[0] == '4') {
        write(sockfd, "pass ", 5);
        write(sockfd, pass, strlen(pass));
        write(sockfd, "\n", 1);
        readServerResponse(sockfd, localResponse, localFullResponse);
    }

    if(strncmp(localResponse, "230", RESPONSE_SIZE) != 0) {
        fprintf(stderr,"Password not accepted\n");
            return -2;
    }

    return 0;
}

int activatePassiveMode(int sockfd) {
    char response[4];
    char fullResponse[512];
    char number[4];
    int i = 0;
    int pasv_numbers[6];
    int j = 0;
    int k = 0;

    write(sockfd, "pasv\n", 5);

    pasv_state state = pasv_start;
    char character;

    memset(fullResponse,0,sizeof(fullResponse));

    while(state != pasv_end) {
        read(sockfd, &character, 1);
        fullResponse[k] = character;
        k++;

        switch(state) {
        case pasv_start:
            if (character == '(') {
                state = h1;
                memset(number,0,sizeof(number));
                }
            break;
        case h1:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == ',') {
                state = h2;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
                memset(number,0,sizeof(number));
                i = 0;
                j++;
            }
            break;
        case h2:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == ',') {
                state = h3;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
                memset(number,0,sizeof(number));
                i = 0;
                j++;
            }
            break;
        case h3:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == ',') {
                state = h4;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
                memset(number,0,sizeof(number));
                i = 0;
                j++;
            }
            break;
        case h4:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == ',') {
                state = p1;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
                memset(number,0,sizeof(number));
                i = 0;
                j++;
            }
            break;
        case p1:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == ',') {
                state = p2;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
                memset(number,0,sizeof(number));
                i = 0;
                j++;
            }
            break;
        case p2:
            if (isdigit(character)) {
                number[i] = character;
                i++;
            }
            else if (character == '\n') {
                state = pasv_end;
                number[3] = '\0';
                pasv_numbers[j] = atoi(number);
            }
            break;
        }
    }

    int port = pasv_numbers[4]*256 + pasv_numbers[5];

    fullResponse[k] = '\0';
    printf("Server Response: \n%s\n", fullResponse);

    return port;
}

int download_file(int sockfd, int sockfd_client, char* file_path) {
    char response[RESPONSE_SIZE];
    char fullResponse[BUFFER_SIZE];
    
    char* filename;

    response[0] = '4';

    while(response[0] == '4') {
        write(sockfd, "retr ", 5);
        write(sockfd, file_path, strlen(file_path));
        write(sockfd, "\n", 1);
        readServerResponse(sockfd, response, fullResponse);
    }

    if (strncmp(response, "150", 3) != 0) {
        fprintf(stderr,"Couldn't open file\n");
        return -1;
    }


    filename = basename(file_path);

    FILE *file = fopen(filename, "wb+");
    
    char file_part[BUFFER_SIZE];
    int bytes_read, elems_written;

    while((bytes_read = read(sockfd_client, file_part, BUFFER_SIZE)) > 0) {
        elems_written = fwrite(file_part, bytes_read, 1, file);
        if (elems_written != 1) {
        fprintf(stderr,"Error downloading file\n");
            return -2;
        }
    }

    fclose(file);

    return 0;
}