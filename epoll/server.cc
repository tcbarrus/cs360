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
#include <string>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         1024
#define HTML                ".html"
#define GIF                 ".gif"
#define JPG                 ".jpg"
#define QUEUE_SIZE          5
#define MAX_PATH_SIZE       100

using namespace std;

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

void serveFile(char* fullPath, int hSocket){
    struct stat filestat; 
    char pBuffer[BUFFER_SIZE];
    if(stat(fullPath, &filestat)) {
        printf("ERROR in stat\n");
        sprintf(pBuffer, "HTTP/1.1 404 File Not Found\r\n\r\n<html><h1>404 Error - File Not Found</h1></html>");
    }
    if(S_ISREG(filestat.st_mode)) {
        memset(pBuffer, 0, sizeof(pBuffer));
        char *buffer = (char *)malloc(filestat.st_size);
        int size = filestat.st_size;
        if(strcmp(getExt(fullPath), JPG) == 0 || strcmp(getExt(fullPath), GIF) == 0){
            readImage(buffer, fullPath, size);
        }
        else{
            readText(buffer, fullPath, size);
        }
        setHeaders(pBuffer, fullPath, size);
        write(hSocket,pBuffer,strlen(pBuffer));
        if(strcmp(getExt(fullPath), JPG) == 0 || strcmp(getExt(fullPath), GIF) == 0){
            write(hSocket, buffer, filestat.st_size);
        }
        else{
            write(hSocket,buffer,strlen(buffer));   
        }
        free(buffer);
    }
    if(S_ISDIR(filestat.st_mode)) {
        memset(pBuffer, 0, sizeof(pBuffer));
        DIR *dirp;
        struct dirent *dp;
        dirp = opendir(fullPath);
        string dirList = "";

        sprintf(pBuffer, "<html><h1>Index of %s</h1></html>\
            <table><tbody>\
            <tr><th valign=\"top\">Name</th></tr>", fullPath);
        dirList.append(pBuffer);
        //write(hSocket, pBuffer, strlen(pBuffer));
        while ((dp = readdir(dirp)) != NULL){
            memset(pBuffer, 0, sizeof(pBuffer));
            if(strcmp(dp->d_name, "index.html") == 0){
                char f[BUFFER_SIZE];
                strcpy(f, fullPath);
                strcat(f, "index.html");
                serveFile(f, hSocket);
                return;
            }else if(strcmp(dp->d_name,".") == 0){
                sprintf(pBuffer, "<tr><td><a href=\"/\">Root</a></td></tr>");
                //result.append("<li><a href=\"/\">Root</a></li>");
            }else if(strcmp(dp->d_name,"..")){
                if(dp->d_type == 0x8){
                    sprintf(pBuffer, "<tr><td><a href=\"%1$s\">%1$s/</a></td></tr>", dp->d_name);
                }else{
                    sprintf(pBuffer, "<tr><td><a href=\"%1$s/\">%1$s/</a></td></tr>", dp->d_name);
                }
            }
            dirList.append(pBuffer);
            //write(hSocket, pBuffer, strlen(pBuffer));
        }
        memset(pBuffer, 0 , sizeof(pBuffer));
        sprintf(pBuffer+strlen(pBuffer), "</tbody></table></html>");
        // sprintf(pBuffer, "HTTP/1.1 200 OK\r\n\r\n");
        dirList.append(pBuffer);
        //write(hSocket, pBuffer, strlen(pBuffer));
        
        memset(pBuffer, 0, sizeof(pBuffer));
        sprintf(pBuffer, "HTTP/1.1 200 OK\r\n\r\n");
        write(hSocket, pBuffer, strlen(pBuffer));
        write(hSocket, dirList.c_str(), dirList.size());
        //write(hSocket, pBuffer, strlen(pBuffer));
        (void)closedir(dirp);
    }
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
    string rootDir;

    // int rc1, rc2;

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
        rootDir=argv[2];
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

        string fullPath = rootDir + path;
        printf("ROOT: %s\n", rootDir.c_str());
        printf("PATH: %s\n", fullPath.c_str());

        serveFile(const_cast<char*>(fullPath.c_str()), hSocket);

        memset(pBuffer, 0, sizeof(pBuffer));

        //Get all responses with epoll 
        for(int i = 0; i < NCONNECTIONS; i++){
            struct epoll_event;
            int rval = epoll_wait(epollfd, &event, 1, -1);
            char buff[10000];
            read(event.data.fd, buff, 10000);

            printf("\nClosing the socket");
            /* close socket */
            if(close(hSocket[i]) == SOCKET_ERROR){
             printf("\nCould not close socket\n");
             return 0;
            }
        }
        
        
    }
}