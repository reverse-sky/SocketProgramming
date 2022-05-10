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
#include<time.h>

#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 9984

char* get_localtime(time_t* rawtime, struct tm*timeinfo){
    time (&rawtime);
    timeinfo = localtime (&rawtime);    
    return asctime(timeinfo);
}


int main ()
{
    time_t rawtime;
    struct tm * timeinfo;
    struct sockaddr_in server,client;
    char buf[1024];
    char dumy[1024];
    int client_len;
    
    int length = sizeof(buf);
    int s,sd;

    char *s1 = "\\service 1";
    char *s2 = "\\service 2";
    char *s3 = "\\service 3";
    char *ServiceList = "\n[Service List]\n1. Get Current Time\n2. Download File\n3. Echo Server\nEnter: ";
    char *AvailableList = "\n[Available File List]\n1.Book.txt\n2.HallymUniv.jpg\n3.Go back\nEnter: ";
    char *ECHO = "Start Echo Service  ";
    if ((s = socket(AF_INET,SOCK_STREAM,0))==-1){perror("socket"); exit(1);}
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
// server.sin_addr.s_addr = htonl(INADDR_ANY);

    server.sin_port = htons(TIME_PORT);

    if (bind(s,(struct sockaddr *)&server  ,sizeof(server))){perror("bind");exit(1);}
    if(listen(s,5)){perror("listen");exit(1);}
    printf("Server Service Start...\n");
    printf("Waiting to connect the client...\n\n");

    if((sd=accept(s,(struct sockaddr *)&client, &client_len))==-1){perror("accept");exit(1);}

    while(1){
        strcpy(buf,ServiceList);
        if(send(sd,buf,length,0)==-1){perror("send");exit(1);} //첫list를 send한다.
        if(recv(sd,buf,length,0)==-1){perror("recv");exit(1);}
        printf("receive: %s\n",buf);
        if ((strcmp(buf,s1))==0){
            printf("start service1: \n");

            strcpy(buf,"Current local time and date: ");
            strcat(buf,get_localtime(rawtime,timeinfo));
            // printf("%s\n",buf);
            if(send(sd,buf,length,0)==-1){perror("send");exit(1);}
            printf("send \n");
        }
        if ((strcmp(buf,s2))==0){
            printf("start service 2: \n");
            strcpy(buf,AvailableList);
            if(send(sd,buf,length,0)==-1){perror("send");exit(1);}
            if(recv(sd,buf,length,0)==-1){perror("recv");exit(1);}
            if (strcmp(buf,"3")==0){
                continue;
            }
            FILE *fp;
            if (strcmp(buf,"1")==0){
                fp = fopen("Book.txt","r");
            }
           if (strcmp(buf,"2")==0){
                fp = fopen("./HallymUniv.jpg", "rb");
            }
            int nread,cnt,size ;
            fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
            size = ftell(fp); 
            fseek(fp, 0, SEEK_SET);    // 파일 포인터를 파일의 끝으로 이동시킴}
            sprintf(buf,"%d",size);//숫자를 문자열로 변환
            if(send(sd,buf,length,0)==-1){perror("send");exit(1);}

            nread =0;
            while(nread<size)   //읽을 때는 buffer-1로 읽음 
            {
                int n = fread(buf, 1,sizeof(buf), fp);
                if(n<0)break;
                if(send(sd,buf,n,0)==-1){perror("send");exit(1);}
                        // fwrite((void*)arr, sizeof(char), cnt, cpy);
                nread+=n;

            }
            // printf("%d \t%d\n",nread,size);
            sleep(1);
            fclose(fp);
        }
        if(strcmp(buf,s3)==0){
            printf("start service 3: \n");

            if(send(sd,ECHO,length,0)==-1){perror("send");exit(1);}
            while(1){
                if(recv(sd,buf,length,0)==-1){perror("recv");exit(1);}
                // printf("%s\n",buf);
                if(strcmp(buf,"\\quit")==0)break;
                strcpy(dumy,"You said....");
                strcat(dumy,buf);
                printf("%s",dumy);
                if(send(sd,dumy,length,0)==-1){perror("send");exit(1);}//받자마자 바로 보냄
            }
            printf("exit\n");
        }
        continue;


        // if (strcmp(buf,"\\quit")==0){break;}

    }
    close(sd);
    printf("서버 종료");
    

  return 0;
}

