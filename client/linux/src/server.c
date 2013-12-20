#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int uinputPipe[2]; 
    char sendBuff[1025];
    time_t ticks;
    pid_t child_pid;
    int execResult;
    char buffer[255];
	int i = 0;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8081);
    
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    listen(listenfd, 10);
	
    connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);    
    pipe(uinputPipe);
    child_pid = fork();
    if (child_pid == 0)
    {
        dup2(uinputPipe[0], STDIN_FILENO);
        execResult = execv("./bin/uinput", NULL);
        perror("No exec");
    }
    else 
    {
        while (1)
        {
            int result = read(connfd, buffer, 255);
	    buffer[result] = '\0';
            write(uinputPipe[1], buffer, result);
        }
        close(listenfd);
       
    }
     

}

