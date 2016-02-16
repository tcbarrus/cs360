#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255
#define MAXMSG              1024

using namespace std;

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    int option;
    int count=1;
    string url;
    bool debug = false;
    int downloadCount = 0;

    while ((option = getopt(argc,argv,"dc:")) != -1) {
        switch (option) {
            case 'd':
                debug = true;
                break;
            case 'c':
                count = atoi(optarg);
                //printf("\nCount is %d",count);
                break;
            default:
                printf("\nUsage: [-d request and headers] [-c count] client host-name host-port url\n");
                exit(EXIT_FAILURE);
        }
    }
    if(argv[optind] == NULL || argv[optind+1] == NULL || argv[optind+2] == NULL){
        printf("\nUsage: [-d request and headers] [-c count] client host-name host-port url\n");
        return 0;
    }
    else{
        strcpy(strHostName,argv[optind]);
        nHostPort=atoi(argv[optind+1]);
        url=argv[optind+2];
        //printf("\nConnecting to %s on port %d to url %s",strHostName,nHostPort, url.c_str());
    }

    //printf("\nMaking a socket");
    /* make a socket */
    for(int i = 0; i < count; i++){
        hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        if(hSocket == SOCKET_ERROR)
        {
            perror("Error in creating socket");
            continue;
        }

        /* get IP address from name */
        pHostInfo=gethostbyname(strHostName);

        if(pHostInfo == 0){
            printf("\nNonexistant host\n");
            exit(1);
        }
        /* copy address into long */
        memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

        /* fill address struct */
        Address.sin_addr.s_addr=nHostAddress;
        Address.sin_port=htons(nHostPort);
        Address.sin_family=AF_INET;

        //printf("\nConnecting to %s (%lX) on port %d",strHostName,nHostAddress,nHostPort);
        /* connect to host */
        if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
           == SOCKET_ERROR)
        {
            perror("Error in connecting to host");
            continue;
        }

        char *message = (char *)malloc(MAXMSG);
        sprintf(message, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n", url.c_str(), strHostName, nHostPort);
        if(debug)
            printf("\nMessage: \n%s\n", message);
        write(hSocket, message, strlen(message));
        memset(pBuffer, 0, BUFFER_SIZE);
        
        string response = "";
        int len;
        while((len = read(hSocket,pBuffer,BUFFER_SIZE)) != 0){
            if(len == -1){
                perror("Read failed");
                exit(1);
            }
            response.append(pBuffer);
            memset(pBuffer, 0, BUFFER_SIZE);
        }

        //printf("Response: \n%s", response.c_str());
        int index = response.find("\r\n\r\n") + 4;
        string header = response.substr(0, index);
        int body_begin = index;
        if(debug)
            printf("Header: \n%s", header.c_str());

        //Parse out content length
        index = response.find("Content-Length");
        index = response.find(":", index)+2;
        int length = stoi(response.substr(index, response.find("\n", index) - index));
        //printf("Content-Length: \n%d", length);
        //Read body
        char *body = (char *)malloc(length);
        sprintf(body, "%s", response.substr(body_begin).c_str());
        if(count == 1)
            printf("%s", body);

        //printf("\nClosing socket\n");
        /* close socket */                       
        if(close(hSocket) == SOCKET_ERROR)
        {
            perror("Error in closing socket");
            continue;
        }
        downloadCount++;
        free(message);
        free(body);
    }
    if(count > 1)
        printf("\nSuccessfully read file %d times\n", downloadCount);
}