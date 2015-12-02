//
//  client.c
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
#include <ctype.h>
#define UDPA 21651
#define UDPB 22651
#define UDPC 23651
#define UDPD 24651

#define TCP 25651
#define VERTEX 4
#define MAX_VERTEX_NUM 20
//图的定义
typedef struct
{
    int vertexNum;
    int edgeNum;
    char vertex[MAX_VERTEX_NUM];
    int arc[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
}Graph,*PGraph;

//辅助数组元素
typedef struct
{
    int from;
    int to;
    int weight;
    int flag;
}ArrayNode;


struct router {
    char name[20];
    char cost[20];
};

int matrix[4][4];
int edge;
int path[4][4];

//construct itoa fuction
//reverse string
char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;
    
    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

/*
 * function：change integer to string
 * char s[] for store
 */
char *my_itoa(int n)
{
    int i = 0,isNegative = 0;
    static char s[100];      //必须为static变量，或者是全局变量
    if((isNegative = n) < 0) //如果是负数，先转为正数
    {
        n = -n;
    }
    do      //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //如果是负数，补上负号
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //最后加上字符串结束符
    return reverse(s);
}

/*
 * function：client first boost up and as a server in tcp protocol, it will receive information from the 4 servers.
 */
void buildTCP()
{
    
    
    int server_sockfd; //服务器端套接字
    int client_sockfd;  //客户端套接字
    int len = 1;
    struct sockaddr_in my_addr;//服务器网络地址结构体
    struct sockaddr_in remote_addr; //客户端网络结构体
    int sin_size;
    char buf[BUFSIZ]; //数据传送的缓冲区
    
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    memset(&my_addr, 0, sizeof(my_addr));//数据初始化--清零
    pptr= hptr->h_addr_list;

    
    memset(&my_addr, 0, sizeof(my_addr));//数据初始化--清零
    my_addr.sin_family = PF_INET;//设置为IP通信
    my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
    my_addr.sin_port = htons(TCP); //服务器端口号
    
    printf("The Client has TCP port number: %d  and IP address: %s \n", TCP, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));

    
    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((server_sockfd = socket(PF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("socket fail ! \r\n");
        //return -1;
    }
    //printf("socket ok !\r\n");
    
    /*将套接字绑定到服务器的网络地址上*/
    if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        printf("bind fail !\r\n");
        //return -1;
    }
    //printf("bind ok !\r\n");
    
    
    
    /*监听连接请求--监听队列长度为5*/
    listen(server_sockfd,5);
    //printf("listen ok\r\n");
    
    sin_size=sizeof(struct sockaddr_in);
    int num = 0;
    while (num < 4)
    {
    
        if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size)) > 0)
        printf("");
        struct router server[4] = {};
        int No = 0;
    while(1)
    {
        
        len=recv(client_sockfd,buf,BUFSIZ,0);
        if(len <= 0)
        {
            break;
        }
        //printf("%s", "!!!!!!");
        //if( len >= 0)//接收服务器端信息, 说明buf里面没东西
        //printf("recerive ok\r\n");
        //printf("%s!!!!!!!!!\n", buf);
        
        buf[len]=' ';
        //printf("%s",buf); //打印服务器端信息
        char *ptr;
        
        ptr = strtok(buf, " ");
        strcpy(server[No].name, ptr);
        ptr = strtok(NULL, "\n");
        char *pTmp = ptr;
        char buffer[30];
        int j = 0;
        while (*pTmp)
        {
            if(*pTmp != ' ')
            {
                buffer[j] = *pTmp;
                j++;
            }
            pTmp++;
        }
        buffer[j] = 0;
        strcpy(server[No].cost, buffer);
        //printf("%s\n", "接受了");
        if(strcmp(server[No].name, "serverA") == 0)  matrix[num][0] = atoi(server[No].cost);
        if(strcmp(server[No].name, "serverB") == 0) matrix[num][1] = atoi(server[No].cost);
        if(strcmp(server[No].name, "serverC") == 0) matrix[num][2] = atoi(server[No].cost);
        if(strcmp(server[No].name, "serverD") == 0) matrix[num][3] = atoi(server[No].cost);
        
        No ++;
        
        
    }
        if (num == 0)
        {
            getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
            printf("The Client receives neighbor information from the ServerA with TCP port number: %d and IP address: %s \n",remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            printf("The ServerA has the following neighbor information: \n");
            printf("Neighbor-----Cost \n");
            while(No >= 0)
            {
                printf("%s     %s \n ",server[No].name,server[No].cost);
                No -- ;
            }
            
        }
        if (num == 1)
        {
            getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
            printf("The Client receives neighbor information from the ServerB with TCP port number: %d and IP address: %s \n",remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            printf("The ServerB has the following neighbor information: \n");
            printf("Neighbor-----Cost \n");
            while(No >= 0)
            {
                printf("%s     %s \n ",server[No].name,server[No].cost);
                No -- ;
            }
            
        }
        if (num == 2)
        {
            getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
            printf("The Client receives neighbor information from the ServerC with TCP port number: %d and IP address: %s \n",remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            printf("The ServerC has the following neighbor information: \n");
            printf("Neighbor-----Cost \n");
            while(No >= 0)
            {
                printf("%s     %s \n ",server[No].name,server[No].cost);
                No -- ;
            }
            
        }
        if (num == 3)
        {
            getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
            printf("The Client receives neighbor information from the ServerD with TCP port number: %d and IP address: %s \n",remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            printf("The ServerD has the following neighbor information: \n");
            printf("Neighbor-----Cost \n");
            while(No >= 0)
            {
                printf("%s     %s \n ",server[No].name,server[No].cost);
                No -- ;
            }
            
        }
        close(client_sockfd);
        num ++;
    }
    close(server_sockfd);
    
    
}


/*
 * function：client build udp for transmitting information to each server
 */
void buildUDPA()
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    int sin_size;
    //char buf[BUFSIZ];//数据传送的缓冲区
    char temp[1000];
  
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    
    
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=PF_INET; //设置为IP通信
    pptr= hptr->h_addr_list;
    remote_addr.sin_family=PF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr  =  inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址
    remote_addr.sin_port=htons(UDPA); //服务器端口号
    
    /*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket");
        //return 1;
    }
    printf("socket ok !\r\n");
    //strcpy(buf,"This is a test message");
    
    //int i =0;
    /*
     for(int j = 0; j < 4; j++)
     {
     //my_itoa(matrix[0][j]);
     //IntToStr(matrix[0][j],temp);
     //printf("%d \n", matrix[0][j]);
     //printf("%s \n",temp);
     if((len=sendto(client_sockfd,my_itoa(matrix[0][j]),strlen(my_itoa(matrix[0][j])),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
     {
     perror("recvfrom");
     return 1;
     }
     }
     */
    //printf("%s",temp);
    sin_size=sizeof(struct sockaddr_in);
    
    
    /*向服务器发送数据包*/
    int i , j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if((len= sendto(client_sockfd,my_itoa(matrix[i][j]),strlen(my_itoa(matrix[i][j]))+1,0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                //return 1;
            }
            
        }
        
        
    }
    printf("The Client has sent the network topology to the network toplogy to the  Server A with UDP port number: %d and IP address: %s \n", UDPA, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    printf("Edge------Cost\n");
    
    if(matrix[0][1] != 0)
    {
        printf("AB     %d\n",matrix[0][1]);
        edge ++;
        
    }
    if(matrix[0][2] != 0)
    {
        printf("AC     %d\n",matrix[0][2]);
        edge ++;
    }
    if(matrix[0][3] != 0)
    {
        printf("AD     %d\n",matrix[0][3]);
        edge ++;
    }
    if(matrix[1][2] != 0)
    {
        printf("BC     %d\n",matrix[1][2]);
        edge ++;
    }
    if(matrix[1][3] != 0)
    {
        printf("BD     %d\n",matrix[1][3]);
        edge ++;
    }
    if(matrix[2][3] != 0)
    {
        printf("CD     %d\n",matrix[2][3]);
        edge ++;
    }
    
    getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
    
    
    printf("For this connection with Server A, The Client has UDP port number: %d and IP address: %s \n", remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    
    
    close(client_sockfd);
   
}

/*
 * function：client build udp for transmitting information to each server
 */
void buildUDPB()
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    int sin_size;
    char buf[BUFSIZ];//数据传送的缓冲区
    char temp[1000];
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    
    
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=PF_INET; //设置为IP通信
    //remote_addr.sin_addr.s_addr=inet_addr("10.123.41.199");//服务器IP地址
    pptr= hptr->h_addr_list;
    remote_addr.sin_family=PF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址
    remote_addr.sin_port=htons(UDPB); //服务器端口号

    
    /*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket");
        //return 1;
    }
    //strcpy(buf,"This is a test message");
    
    //int i =0;
    /*
     for(int j = 0; j < 4; j++)
     {
     //my_itoa(matrix[0][j]);
     //IntToStr(matrix[0][j],temp);
     //printf("%d \n", matrix[0][j]);
     //printf("%s \n",temp);
     if((len=sendto(client_sockfd,my_itoa(matrix[0][j]),strlen(my_itoa(matrix[0][j])),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
     {
     perror("recvfrom");
     return 1;
     }
     }
     */
    //printf("%s",temp);
    sin_size=sizeof(struct sockaddr_in);
    /*向服务器发送数据包*/
    int i, j;
    for(i = 0; i < 4; i++)
    {
        for( j = 0; j < 4; j++)
        {
            if((len= sendto(client_sockfd,my_itoa(matrix[i][j]),strlen(my_itoa(matrix[i][j]))+1,0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                //return 1;
            }
            
        }
        
        
    }
    printf("The Client has sent the network topology to the network toplogy to the  Server B with UDP port number: %d and IP address: %s \n", UDPB, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    printf("Edge------Cost\n");
    
    if(matrix[0][1] != 0) printf("AB     %d\n",matrix[0][1]);
    if(matrix[0][2] != 0) printf("AC     %d\n",matrix[0][2]);
    if(matrix[0][3] != 0) printf("AD     %d\n",matrix[0][3]);
    
    if(matrix[1][2] != 0) printf("BC     %d\n",matrix[1][2]);
    if(matrix[1][3] != 0) printf("BD     %d\n",matrix[1][3]);
    
    if(matrix[2][3] != 0) printf("CD     %d\n",matrix[2][3]);
    
    getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
    
    
    printf("For this connection with Server B, The Client has UDP port number: %d and IP address: %s \n", remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    
    
    close(client_sockfd);
}

/*
 * function：client build udp for transmitting information to each server
 */
void buildUDPC()
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    int sin_size;
    char buf[BUFSIZ];//数据传送的缓冲区
    char temp[1000];
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    
    
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=PF_INET; //设置为IP通信
    //remote_addr.sin_addr.s_addr=inet_addr("10.123.41.199");//服务器IP地址
    pptr= hptr->h_addr_list;
    remote_addr.sin_family=PF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址
    remote_addr.sin_port=htons(UDPC); //服务器端口号

    
    /*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket");
        //return 1;
    }
    //strcpy(buf,"This is a test message");
    
    //int i =0;
    /*
     for(int j = 0; j < 4; j++)
     {
     //my_itoa(matrix[0][j]);
     //IntToStr(matrix[0][j],temp);
     //printf("%d \n", matrix[0][j]);
     //printf("%s \n",temp);
     if((len=sendto(client_sockfd,my_itoa(matrix[0][j]),strlen(my_itoa(matrix[0][j])),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
     {
     perror("recvfrom");
     return 1;
     }
     }
     */
    //printf("%s",temp);
    sin_size=sizeof(struct sockaddr_in);
    /*向服务器发送数据包*/
    int i , j;
    for( i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if((len= sendto(client_sockfd,my_itoa(matrix[i][j]),strlen(my_itoa(matrix[i][j]))+1,0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                //return 1;
            }
            
        }
        
        
    }
    printf("The Client has sent the network topology to the network toplogy to the  Server C with UDP port number: %d and IP address: %s \n", UDPC, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    printf("Edge------Cost\n");
    
    if(matrix[0][1] != 0) printf("AB     %d\n",matrix[0][1]);
    if(matrix[0][2] != 0) printf("AC     %d\n",matrix[0][2]);
    if(matrix[0][3] != 0) printf("AD     %d\n",matrix[0][3]);
    
    if(matrix[1][2] != 0) printf("BC     %d\n",matrix[1][2]);
    if(matrix[1][3] != 0) printf("BD     %d\n",matrix[1][3]);
    
    if(matrix[2][3] != 0) printf("CD     %d\n",matrix[2][3]);
    
    getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
    
    
    printf("For this connection with Server C, The Client has UDP port number: %d and IP address: %s \n", remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    
    
    close(client_sockfd);
}

/*
 * function：client build udp for transmitting information to each server
 */
void buildUDPD()
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    int sin_size;
    char buf[BUFSIZ];//数据传送的缓冲区
    char temp[1000];
    
    char **pptr;
    char str[32];
    struct hostent * hptr = gethostbyname("localhost");
    
    
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=PF_INET; //设置为IP通信
    //remote_addr.sin_addr.s_addr=inet_addr("10.123.41.199");//服务器IP地址
    pptr= hptr->h_addr_list;
    remote_addr.sin_family=PF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr(inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));//服务器IP地址
    remote_addr.sin_port=htons(UDPD); //服务器端口号

    
    /*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket");
        //return 1;
    }
    //strcpy(buf,"This is a test message");
    
    //int i =0;
    /*
     for(int j = 0; j < 4; j++)
     {
     //my_itoa(matrix[0][j]);
     //IntToStr(matrix[0][j],temp);
     //printf("%d \n", matrix[0][j]);
     //printf("%s \n",temp);
     if((len=sendto(client_sockfd,my_itoa(matrix[0][j]),strlen(my_itoa(matrix[0][j])),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
     {
     perror("recvfrom");
     return 1;
     }
     }
     */
    //printf("%s",temp);
    sin_size=sizeof(struct sockaddr_in);
    
    /*向服务器发送数据包*/
    int i , j;
    for(i = 0; i < 4; i++)
    {
        for( j = 0; j < 4; j++)
        {
            if((len= sendto(client_sockfd,my_itoa(matrix[i][j]),strlen(my_itoa(matrix[i][j]))+1,0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                //return 1;
            }
            
        }
        
        
    }
    printf("The Client has sent the network topology to the network toplogy to the  Server D with UDP port number: %d and IP address: %s \n", UDPD, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    printf("Edge------Cost\n");
    
    if(matrix[0][1] != 0) printf("AB     %d\n",matrix[0][1]);
    if(matrix[0][2] != 0) printf("AC     %d\n",matrix[0][2]);
    if(matrix[0][3] != 0) printf("AD     %d\n",matrix[0][3]);
    
    if(matrix[1][2] != 0) printf("BC     %d\n",matrix[1][2]);
    if(matrix[1][3] != 0) printf("BD     %d\n",matrix[1][3]);
    
    if(matrix[2][3] != 0) printf("CD     %d\n",matrix[2][3]);
    
    getsockname(client_sockfd, (struct sockaddr *) &remote_addr, (socklen_t *) &sin_size);
    
    
    printf("For this connection with Server D, The Client has UDP port number: %d and IP address: %s \n", remote_addr.sin_port, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    
    
    close(client_sockfd);

}

/*
 * function：create a graph for calcualte MST
 */
void createdGraph(PGraph g)
{
    int i,j,k,o;
    g->vertexNum= VERTEX;
    g->edgeNum= edge;
    
    for(i=0;i<g->vertexNum;i++)
        g->vertex[i]='A'+i;
    for(i=0;i<g->vertexNum;i++)
        for(j=0;j<g->vertexNum;j++)
            g->arc[i][j]=0;
    
    for(k = 0; k < 4; k++)
    {
        for( o = 0;o < 4; o++)
        {
            if(matrix[k][o] != 0)
            {
                g -> arc[k][o] = matrix[k][o];
            }
        }
    }
}

/*
 * function：initial a tree
 */
void initTree(PGraph tree)
{
    int i,j;
    tree->vertexNum= VERTEX;
    tree->edgeNum= edge;
    for(i=0;i<tree->vertexNum;i++)
        tree->vertex[i]='0';
    for(i=0;i<tree->vertexNum;i++)
        for(j=0;j<tree->vertexNum;j++)
            tree->arc[i][j]=0;

}

/*
 * function：use prime algorithm to calcaulate
 * reference: Book 《Algorithm Design -- JON KLEINBERG》 and Internet as :http://blog.csdn.net/qiaoruozhuo/article/details/41515089?utm_source=tuicool&utm_medium=referral
 */
void prim(PGraph g,PGraph tree)
{
    int i,j,k;
    int index;  //指向权值最小的边
    ArrayNode  edgeArray[MAX_VERTEX_NUM*2]; //辅助数组
    int length=0; //数组长度
    int n=1;  //统计数组已加入多少个顶点
    int cost=0;
    
    //初始状态把第一个顶点加入树中
    tree->vertex[0]='A';
    //printf("%-3c",tree->vertex[0]);
    i=0;
    while(1){
        //寻找与顶点i相接且这条边的另一个顶点不在树中的边,存入edgeArray数组中
        for(j=0;j<g->vertexNum;j++){
            if(g->arc[i][j] > 0){
                //判断这条边的另一个顶点在不在树中
                for(k=0;k<tree->vertexNum;k++){
                    if(tree->vertex[k] == g->vertex[j])
                        break;
                }
                if(k == tree->vertexNum){
                    edgeArray[length].from=i;
                    edgeArray[length].to=j;
                    edgeArray[length].weight=g->arc[i][j];
                    edgeArray[length].flag=0;
                    length++;
                }
            }
        }
        //从数组中选择权值最小的边
        index=-1;
        for(j=0;j<length;j++){
            if(index == -1 && edgeArray[j].flag == 0)
                index=j;
            if(edgeArray[j].flag==0 && edgeArray[j].weight < edgeArray[index].weight)
                index=j;
        }
        //在树中加入一个顶点，且把这条权值最小的边加入树中
        tree->vertex[edgeArray[index].to]='A'+edgeArray[index].to;
        edgeArray[index].flag=1;
        tree->arc[edgeArray[index].from][edgeArray[index].to]=edgeArray[index].weight;
        tree->arc[edgeArray[index].to][edgeArray[index].from]=edgeArray[index].weight;
        path[edgeArray[index].from][edgeArray[index].to] = edgeArray[index].weight;
        cost = cost + edgeArray[index].weight;
        //当这个顶点加入树中时，与这个顶点相邻的边不可加入树中
        for(k=0;k<length;k++){
            if(edgeArray[k].to == edgeArray[index].to)
                edgeArray[k].flag=1;
        }
        i=edgeArray[index].to;
        //printf("%-3c",tree->vertex[i]);
        n++;
        //当有g->vertexNum个顶点时，最小生成树构造完成

        if(n==g->vertexNum)
        {
         //   printf(" %d", cost);
            break;
        }
    }
    printf("The Client has calculated a tree. The tree cost is %d \n", cost);
    printf("Edge------Cost\n");
    if(path[0][1] != 0) printf("AB     %d\n",matrix[0][1]);
    if(path[0][2] != 0) printf("AC     %d\n",matrix[0][2]);
    if(path[0][3] != 0) printf("AD     %d\n",matrix[0][3]);
    
    if(path[1][2] != 0) printf("BC     %d\n",matrix[1][2]);
    if(path[1][3] != 0) printf("BD     %d\n",matrix[1][3]);
    
    if(path[2][3] != 0) printf("CD     %d\n",matrix[2][3]);
    
}



int main(int argc, char *argv[])
{

    buildTCP();
    
    buildUDPA();
    buildUDPB();
    buildUDPC();
    buildUDPD();
    
    Graph graph;
    Graph tree;
    createdGraph(&graph);
    initTree(&tree);
    prim(&graph,&tree);
    printf("\n");

    return 0;


}

