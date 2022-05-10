#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<inttypes.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/un.h>
#include<sys/uio.h>

#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 9984

int main(){
    char *s1 = "\\service 1";
    char *s2 = "\\service 2";
    char *s3 = "\\service 3";
    struct sockaddr_in addr;
    char buf[1024];
    int length = sizeof(buf);
    int s;
    char *command = "\\quit";

    if((s = socket(AF_INET,SOCK_STREAM,0))==-1)exit(1);
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = htonl(inet_addr(TIME_SERVER));
    addr.sin_addr.s_addr = inet_addr(TIME_SERVER);

    addr.sin_port = htons(TIME_PORT);
    
    if (connect(s,(struct sockaddr*)&addr,sizeof(addr))==-1){perror("connect");exit(1);}
    //연결 됬다고 가정 
    printf("Connet the Server...\n\n\n");

    while(1){
        if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
        printf("%s",buf);
        gets(buf);
        if(send(s,buf,length,0)==-1){perror("send");exit(1);}
        if(strcmp(buf,s1)==0){ //이건 해결 다음 파일 입출력
            if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
            printf("%s",buf);
            // continue;
        }
        if(strcmp(buf,s2)==0){

            if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
            printf("%s",buf);
            gets(buf);
            if(send(s,buf,length,0)==-1){perror("send");exit(1);}
            if(strcmp(buf,"3")==0){
                continue;
            }
            FILE *fp;
            if (strcmp(buf,"1")==0){
                fp = fopen("DownLoad_Book.txt","a");
            }
            if (strcmp(buf,"2")==0){
                fp = fopen("DownLoad_HallymUniv.jpg", "ab");
            }
            int size,nread,rc;
            if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
            size = atoi(buf);//사이즈를 정수로 받음

            nread =0;
            while(nread<size)   //읽을 때는 buffer-1로 읽음 
            {
                // int n = fread(buf, 1,sizeof(buf)-1 , fp);
                if((rc = recv(s,buf,length,0))==-1){perror("recv");exit(1);}
                if (rc<0)break;

                fwrite((void*)buf, sizeof(char), rc,fp);
                nread+=rc;

            }
            // printf("%d \t%d",nread,size);
            fclose(fp);
        }


        if(strcmp(buf,s3)==0){

            if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
            printf("%s",buf);
            while(1){
                gets(buf,100,stdin);


                if(send(s,buf,length,0)==-1){perror("send");exit(1);}
                if(strcmp(buf,"\\quit")==0)break;
                if(recv(s,buf,length,0)==-1){perror("recv");exit(1);}
                printf("%s\nEnter: ",buf);
            }
            // continue;
        }
        else continue;
    
    
   
    }
    return 0;
}