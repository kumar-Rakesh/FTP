#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/dir.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define USER 1
#define PORT 2
#define RETR 3
#define STOR 4
#define LIST 5
#define QUIT 6
#define PWD 7
#define MKD 8
#define CWD 9
#define CDUP 10
#define RMD 11
#define DELE 12
#define NOOP 13
#define APPE 14
#define INVALID -1

#define MAX_SIZE 10000
