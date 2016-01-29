#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         1024
#define HTML                ".html"
#define GIF                 ".gif"
#define JPG                 ".jpg"
#define QUEUE_SIZE          5
#define MAX_PATH_SIZE       100

void extractRequest(char* pBuffer, char* path){
    printf("GOT THIS FROM THE BROWSER: \n %s\n", pBuffer);
    int rval = sscanf(pBuffer, "GET %s HTTP/1.1", path);
}

const char *getExt (const char *fspec) {
    const char *e = strrchr (fspec, '.');
    if (e == NULL)
        e = "";
    return e;
}

void readText(char* buffer, char* path, int size){
    FILE *fp = fopen(path,"r");
    fread(buffer, size, 1, fp);
}

void readImage(char* buffer, char* path, int size){
    FILE *fp = fopen(path,"rb");
    fread(buffer, size, 1, fp);
}

void setHeaders(char* pBuffer, char* path, int length){
    sprintf(pBuffer, "HTTP/1.1 200 OK\n");
    if(strcmp(getExt(path), HTML) == 0){
        sprintf(pBuffer + strlen(pBuffer), "Content-Type: text/html\n");
    }
    else if(strcmp(getExt(path), GIF) == 0){
        sprintf(pBuffer + strlen(pBuffer), "Content-Type: image/gif\n");
    }
    else if(strcmp(getExt(path), JPG) == 0){
        sprintf(pBuffer + strlen(pBuffer), "Content-Type: image/jpg\n");
    }
    else{
        sprintf(pBuffer + strlen(pBuffer), "Content-Type: text/plain\n");
    }
    if(length > 0)
        sprintf(pBuffer + strlen(pBuffer), "Content-Length: %d", length);
    sprintf(pBuffer + strlen(pBuffer), "\r\n\r\n");
}

void handler (int status)
{
    printf("received signal %d\n",status);
}

int main(int argc, char* argv[])
{
    int hSocket,hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    char pBuffer[BUFFER_SIZE];
    char* fullPath;
    int nHostPort;
    struct stat filestat; 

    int rc1, rc2;

    // First set up the signal handler
    // struct sigaction sigold, signew;
    // signew.sa_handler=handler;
    // sigemptyset(&signew.sa_mask);
    // sigaddset(&signew.sa_mask,SIGINT);
    // signew.sa_flags = SA_RESTART;
    // sigaction(SIGINT,&signew,&sigold);
    // sigaction(SIGHUP,&signew,&sigold);
    // sigaction(SIGPIPE,&signew,&sigold);

    if(argc < 3)
      {
        printf("\nUsage: server host-port directory\n");
        return 0;
      }
    else
      {
        nHostPort=atoi(argv[1]);
        fullPath = argv[2];
      }

    printf("\nStarting server");

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);

    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nBinding to port %d",nHostPort);

    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address)) 
                        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
 /*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );

        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );

    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return 0;
    }

    for(;;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);
        int optval = 1;
        setsockopt (hSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        printf("\nGot a connection from %X (%d)\n",
              Address.sin_addr.s_addr,
              ntohs(Address.sin_port));
        memset(pBuffer, 0, sizeof(pBuffer));
        int rval = read(hSocket, pBuffer, BUFFER_SIZE);
        
        char path[MAX_PATH_SIZE];
        extractRequest(pBuffer, path);
        printf("PATH: %s\n", path);
        //Parse out leading /
        // memcpy(path, &path[1], sizeof(path) - 1);
        // if(strlen(path) == 0)
        //     strcpy(path, argv[2]);
        if(stat(path, &filestat)) {
            printf("ERROR in stat\n");
            memset(pBuffer, 0, sizeof(pBuffer));
            sprintf(pBuffer, "HTTP/1.1 404 FILE NOT FOUND\r\n\r\n");
            write(hSocket,pBuffer,strlen(pBuffer));
        }
        if(S_ISREG(filestat.st_mode)) {
            memset(pBuffer, 0, sizeof(pBuffer));
            char *buffer = (char *)malloc(filestat.st_size);
            int size = filestat.st_size;
            if(strcmp(getExt(path), JPG) == 0 || strcmp(getExt(path), GIF) == 0){
                readImage(buffer, fullPath, size);
            }
            else{
                readText(buffer, fullPath, size);
            }
            setHeaders(pBuffer, path, size);
            write(hSocket,pBuffer,strlen(pBuffer));
            if(strcmp(getExt(path), JPG) == 0 || strcmp(getExt(path), GIF) == 0){
                write(hSocket, buffer, filestat.st_size);
            }
            else{
                write(hSocket,buffer,strlen(buffer));   
            }
            free(buffer);
        }
        if(S_ISDIR(filestat.st_mode)) {
            memset(pBuffer, 0, sizeof(pBuffer));
            sprintf(pBuffer, "HTTP/1.1 200 OK\n");//\r\n\r\n");
            sprintf(pBuffer + strlen(pBuffer), "Content-Type: text/html\r\n\r\n");
            write(hSocket, pBuffer, strlen(pBuffer));
            memset(pBuffer, 0, sizeof(pBuffer));
            DIR *dirp;
            struct dirent *dp;
            strcat(fullPath, path);
            dirp = opendir(fullPath);

            sprintf(pBuffer, "<html><h1>Index of %s</h1></html>\
                <table><tbody>\
                <tr><th valign=\"top\">Name</th></tr>", path);
            write(hSocket, pBuffer, strlen(pBuffer));
            while ((dp = readdir(dirp)) != NULL){
                memset(pBuffer, 0, sizeof(pBuffer));
                if(strcmp(dp->d_name, "index.html") == 0){
                    //TODO: return index.html
                }
                if(dp->d_type == 0x8){
                    //Is file
                    sprintf(pBuffer, "<tr><td><a href=\"%s%s\">%s</a></td></td>", path, dp->d_name, dp->d_name);
                }
                else{
                    sprintf(pBuffer,"<tr><td><a href=\"%s%s/\">%s</a></td></td>", path, dp->d_name, dp->d_name);
                }
                write(hSocket, pBuffer, strlen(pBuffer));
            }
            sprintf(pBuffer+strlen(pBuffer), "</tbody></table></html>");
            write(hSocket, pBuffer, strlen(pBuffer));
            (void)closedir(dirp);
        }
        //TODO: If directory, look for index file
        //TODO: If index exists, return index

        memset(pBuffer, 0, sizeof(pBuffer));
        // linger lin;
        // unsigned int y=sizeof(lin);
        // lin.l_onoff=1;
        // lin.l_linger=10;
        // setsockopt(hSocket,SOL_SOCKET, SO_LINGER,&lin,sizeof(lin));
        // shutdown(hSocket, SHUT_RDWR);
        
        printf("\nClosing the socket");
        /* close socket */
        if(close(hSocket) == SOCKET_ERROR){
         printf("\nCould not close socket\n");
         return 0;
        }
    }
}