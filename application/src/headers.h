#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <libgen.h>
#include <sys/time.h>

#define SERVER_PORT 21
#define SERVER_ADDR "192.168.28.96"

#define MAX_SIZE 256
#define RESPONSE_SIZE 3
#define BUFFER_SIZE 1024

typedef enum
{
    inputError = 1,
    parseError = 2,
    domainNameError = 3,
    socketOpeningError = 4,
    socketConnectingError = 5,
    serverConnectionError = 6,
    serverLoginError = 7,
    passiveModeError = 8,
    downloadingError = 9,
    socketClosingError = 10,
} errorType;

typedef enum
{
    start,
    first_digit,
    second_digit,
    third_digit,
    last_line,
    code_received,
} code_state;


typedef enum
{
   pasv_start,
   pasv_end,
   h1,
   h2,
   h3,
   h4,
   p1,
   p2,
} pasv_state;


