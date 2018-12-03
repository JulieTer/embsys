#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <syslog.h>
#include <util.h>

int maxFd(int fd1, int fd2){
  if (fd1>fd2){
    return fd1;
  }else{
    return fd2;
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port1 = NULL;
    char * port2 = NULL;

    openlog ("gps_reader", LOG_CRON | LOG_PID, LOG_LPR);
    syslog (LOG_NOTICE, "test");
    closelog ();

    // parse comand line
    if (argc != 4)
    {
        fprintf(stderr, "Invalid usage: reader -p port_name1 port_name2\n");
        exit(EXIT_FAILURE);
    }

    char * options = "p:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port1 = argv[2];
                port2 = argv[3];
                printf("%s, %s\n", port1, port2);
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // open serial port
    int fd1 = open(port1, O_RDWR | O_NOCTTY);
    int fd2 = open(port2, O_RDWR | O_NOCTTY);
    if (fd1 == -1 || fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd1, TCIOFLUSH);
    tcflush(fd2, TCIOFLUSH);

    // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {
        bzero(buff, sizeof(buff));

        FD_ZERO(&fdset);
        FD_SET(fd1, &fdset);
        FD_SET(fd2, &fdset);

        int fdmax = maxFd(fd1, fd2);
        select(fdmax+1, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(fd1, &fdset))
        {
            int bytes = read (fd1, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("FD1 : %s\n", buff);
                fflush(stdout);
            }
        }
        if (FD_ISSET(fd2, &fdset))
        {
            int bytes = read (fd2, buff, sizeof(buff));

            if (bytes > 0)
            {
                printf("FD2 : %s\n", buff);
                fflush(stdout);
            }
        }
    }

    // close serial port
    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
