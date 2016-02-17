#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <iostream>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define HOST_NAME_SIZE      255
#define MAXMSG              1024

int  main(int argc, char* argv[])
{
    int count;
    bool debug = false;
    int option;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    string url;

    while ((option = getopt(argc,argv,"d")) != -1) {
        switch (option) {
            case 'd':
                debug = true;
                break;
            default:
                printf("\nUsage: [-d request and headers] [-c count] client host-name host-port url\n");
                exit(EXIT_FAILURE);
        }
    }
    if(argv[optind] == NULL || argv[optind+1] == NULL || argv[optind+2] == NULL || argv[optind+3] == NULL){
        printf("\nUsage: [-d request and headers] [-c count] client host-name host-port url\n");
        return 0;
    }
    else{
        strcpy(strHostName,argv[optind]);
        nHostPort=atoi(argv[optind+1]);
        url=argv[optind+2];
        count=atoi(argv[optind+3]);
    }

    int hSocket[count];                 /* handle to socket */
    struct timeval oldtime[count];
    struct timeval newtime[count];
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;

    printf("\nMaking a socket");
    /* make a socket */
    for(int i = 0; i < count; i++) {
        hSocket[i]=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        if(hSocket[i] == SOCKET_ERROR)
        {
            printf("\nCould not make a socket\n");
            return 0;
        }
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    int epollFD = epoll_create(1);
    // Send the requests and set up the epoll data
    for(int i = 0; i < count; i++) {
        /* connect to host */
        if(connect(hSocket[i],(struct sockaddr*)&Address,sizeof(Address)) 
           == SOCKET_ERROR)
        {
            printf("\nCould not connect to host\n");
            return 0;
        }
        // char request[] = "GET /test.html HTTP/1.0\r\n\r\n";
        char *request = (char *)malloc(MAXMSG);
        sprintf(request, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n", url.c_str(), strHostName, nHostPort);

        write(hSocket[i],request,strlen(request));
        struct epoll_event event;
        event.data.fd = hSocket[i];
        event.events = EPOLLIN;
        int ret = epoll_ctl(epollFD,EPOLL_CTL_ADD,hSocket[i],&event);
        if(ret)
            perror ("epoll_ctl");
        gettimeofday(&oldtime[i], NULL);
    }
    for(int i = 0; i < count; i++) {
        struct epoll_event event;
        int rval = epoll_wait(epollFD,&event,1,-1);
        if(rval < 0)
            perror("epoll_wait");
        read(event.data.fd,pBuffer,BUFFER_SIZE);
        gettimeofday(&newtime[i], NULL);
        double usec = (newtime[i].tv_sec - oldtime[i].tv_sec)*(double)1000000+(newtime[i].tv_usec-oldtime[i].tv_usec);
        if(debug)
            std::cout << "Time "<<usec/1000000<<std::endl;
        printf("got from %d\n",event.data.fd);

        printf("\nClosing socket\n");
        /* close socket */                       
        if(close(hSocket[i]) == SOCKET_ERROR)
        {
            printf("\nCould not close socket\n");
            return 0;
        }
    }
}