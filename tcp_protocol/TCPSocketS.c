#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <fcntl.h>

#define MAXLINE 127

int main(int argc, char *argv[]) 
{ 
struct sockaddr_in servaddr, cliaddr; 
int listen_sock, accp_sock, // 
addrlen = sizeof(cliaddr), // 
nbyte, nbuf; 
char buf[MAXLINE+1]; 
char cli_ip[20]; 
char newFileName[20]; 
int filesize=0; 
int total=0, sread, fp;

if(argc != 2) { 
printf("usage : %s port ", argv[0]); 
exit(0); 
} 
// socket creation
 if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
 perror("socket fail"); 
 exit(0); 
 } 
 // servaddr call
 bzero((char *)&servaddr, sizeof(servaddr)); 
 // servaddr setting
  
  servaddr.sin_family = AF_INET; 
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
 servaddr.sin_port = htons(atoi(argv[1]));

 // bind() call
 if(bind(listen_sock, (struct sockaddr *)&servaddr, 
 sizeof(servaddr)) < 0) 
 { 
 perror("bind fail"); 
 exit(0); 
 } 
 // sockets in to waiting mode
 listen(listen_sock, 5);

 while(1) 
 { 
 puts("server is waiting."); 
 // waits for connection
 accp_sock = accept(listen_sock, 
 (struct sockaddr *)&cliaddr, &addrlen);

 if(accp_sock < 0) 
 { 
 perror("accept fail"); 
 exit(0); 
 }

 puts("client has accedssed"); 

 //inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cli_ip, sizeof(cli_ip)); 
 //printf( "IP : %s ", cli_ip );
 // printf( "Port : %x ", ntohs( cliaddr.sin_port) );

 bzero( newFileName, 20 ); 
 recv( accp_sock,newFileName, sizeof(newFileName), 0 ); 
 printf( "%s ", newFileName );

 
 read( accp_sock, &filesize, sizeof(filesize) ); 
 printf( "%d ", filesize );

  fp = open( newFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);

 while( total != filesize ) 
 { 
 sread = recv( accp_sock, buf, 100, 0 ); 
 
 total += sread; 
 buf[sread] = 0; 
 write( fp, buf, sread ); 
 bzero( buf, sizeof(buf) ); 
 printf("donwload processing : %4.2f%%\n\n===========================",total*100/(float)filesize);

 } 
 printf( "FIle transfer has completed\n" ); 
printf("filesize: %d, received: %d\n\n", filesize, total);
 close(fp); 
 close(accp_sock); 
sread=0;
 total=0;

 }

 close( listen_sock ); 
 return 0; 
 }
