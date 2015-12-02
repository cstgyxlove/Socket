//
//  main.c
//  demo
//
//  Created by 世泰 陈 on 15/10/23.
//  Copyright (c) 2015年 世泰 陈. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#define UDP 21651
#define TCP 25651

struct router {
    char name[20];
    char cost[20];
};


/*
 * function：send itself connection to the client
 * The format is from Beej or Internet as http://blog.csdn.net/piaojun_pj/article/details/5920888
 */
void sendmessage() {
    int client_sockfd;
    int len =1 ;
    struct sockaddr_in remote_addr;//服务器端网络地址结构体
    char buf[BUFSIZ];//数据传送的缓冲区
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    
    //    //show the IP address!!!
    //    switch(hptr->h_addrtype)
    //    {
    //        case AF_INET:
    //        case AF_INET6:
    //            pptr=hptr->h_addr_list;
    //            for(; *pptr!=NULL; pptr++)
    //                printf(" address:%s\n",
    //                       inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    //            printf(" first address: %s\n",
    //                   inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
    //            break;
    //        default:
    //            printf("unknown address type\n");
    //            break;
    //    }
    
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    pptr= hptr->h_addr_list;
    remote_addr.sin_family=PF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址
    
    
    int length = sizeof(remote_addr);
    //getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &length);
    //printf("TCP prot number %d ",remote_addr.sin_port);
    remote_addr.sin_port = htons(TCP); //服务器端口号
    
    printf("The Server A is up and running! \n");

    
    FILE *fp;
    int i = 0;
    int name;
    int cost;
    fp = fopen("serverA.txt", "r");
    
    if(fp == NULL)
    {
        printf("CANNOT OPEN \n");
        exit(1);
    }
    //printf("读取了文件 \r\n");
    
    char buffer[1024];
    char message[50];
    char *ptr = NULL;
    int sin_size=sizeof(struct sockaddr_in);
    
    
    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        printf("socket fail ! \r\n");
        //return -1;
    }
    //printf("socket ok !\r\n");
    /*将套接字绑定到服务器的网络地址上*/
    
    
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        perror("connect");
        //return 1;
    }
//    printf("connect ok !\r\n");
//    printf("connected to server \r\n");
    
    // 获取子socket的port！！！！！！！！
    getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &length);
    //printf("TCP prot number %d ",remote_addr.sin_port);
    
  
    printf("Neighbor------Cost\n");
    while(fgets(buffer, 1024, fp) != NULL)
    {
        
        
        printf("%s", buffer);
        int len =send(client_sockfd,buffer,1024,0);
        
    }
    
    printf("\n");
    
    printf("The Server A finishes sending its neighbor information to the Client with TCP port number: %d and IP address: %s \n", TCP, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    
    printf("For this connection with Client, the Server A has TCP port number: %d and IP address: %s \n", remote_addr.sin_port,inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    close(client_sockfd);//关闭套接字
    
    
}


/*
 * function：receive the topology of the whole internet
 *The format is from Beej or Internet as http://blog.csdn.net/piaojun_pj/article/details/5920888
 */
void receiver()
{
    int server_sockfd;
    struct sockaddr_in my_addr;   //服务器网络地址结构体
    struct sockaddr_in remote_addr; //客户端网络地址结构体
    int sin_size;
    //char buf[BUFSIZ];  //数据传送的缓冲区
   
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    

    memset(&my_addr,0,sizeof(my_addr)); //数据初始化--清零
    my_addr.sin_family=PF_INET; //设置为IP通信
    pptr= hptr->h_addr_list;

    my_addr.sin_addr.s_addr=inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址--允许连接到所有本地地址上
    my_addr.sin_port=htons(UDP); //服务器端口号

    
    
    
    //int buf[4];
    int matrix[4][4];
    int i = 0;
    int j = 0;
    /*创建服务器端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((server_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        printf("socket fail ! \r\n");
        //return -1;
    }
    printf("socket ok !\r\n");
    
    /*将套接字绑定到服务器的网络地址上*/
    if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        printf("bind fail !\r\n");
        //return -1;
    }
    //printf("bind ok !\r\n");
    
    sin_size=sizeof(struct sockaddr_in);
    //printf("waiting for a packet...\n");
    
    
    /*接收客户端的数据并将其发送给客户端--recvfrom是无连接的*/
    while(1)
    {
        char buf[BUFSIZ];
        recvfrom(server_sockfd,buf,BUFSIZ,0,(struct sockaddr *)&remote_addr,&sin_size);
        //if(len <= 0) break; 一开始就是len = 0 所以就跳出来了。所以要注释掉，但是为什么呢？？？？？？
        matrix[i][j++] = atoi(buf);
        if(j >= 4)
        {
            i++;
            j = 0;
        }
        if(i >= 4) break;
        
    }
    printf("The Server A has received the network topology from the Client with UDP port number: %d and IP address: %s \n",TCP,inet_ntoa(remote_addr.sin_addr));
    printf("\n");
    printf("Edge------Cost\n");
    
    if(matrix[0][1] != 0) printf("AB     %d\n",matrix[0][1]);
    if(matrix[0][2] != 0) printf("AC     %d\n",matrix[0][2]);
    if(matrix[0][3] != 0) printf("AD     %d\n",matrix[0][3]);
    
    if(matrix[1][2] != 0) printf("BC     %d\n",matrix[1][2]);
    if(matrix[1][3] != 0) printf("BD     %d\n",matrix[1][3]);
    
    if(matrix[2][3] != 0) printf("CD     %d\n",matrix[2][3]);
    
    printf("\n");
    //getsockname(server_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);

    printf("For this connection with Client, The Server A has UDP port number: %d and IP address: %s \n", UDP,  inet_ntoa(my_addr.sin_addr));

    close(server_sockfd);
}




int main(int argc, const char * argv[]) {
    // insert code here...
    int matrix[4][4];
    sendmessage();
    receiver();
    return 0;
}


