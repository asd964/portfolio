#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 127

int main(int argc, char **argv )
{
struct sockaddr_in servaddr;
int s, nbyte;
char buf[MAXLINE+1];
char filename[20];
char newFileName[20];
int filesize, fp, filenamesize , newFileNameSize;
int sread, total=0;

if(argc != 3) 
{
printf("usage: %s ip_address port ", argv[0]);
exit(0);
}

if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
{
perror("socket fail");
exit(0);
}
bzero((char *)&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
servaddr.sin_port = htons(atoi(argv[2]));
if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
{
perror("connection failed");
exit(0);
}

printf("select file to send : ");
if ( fgets(filename, sizeof(filename), stdin) == NULL )
exit(0);
printf("name fot the file that you wish it would be saved as : " );
fgets(newFileName,sizeof(newFileName),stdin);
filenamesize = strlen(filename);
filename[filenamesize-1] = 0;
newFileNameSize = strlen(newFileName);
newFileName[newFileNameSize-1]=0;

if( (fp = open( filename, O_RDONLY )) < 0 )
{
printf( "open failed" );
exit(0);
}

send( s, newFileName, sizeof(newFileName), 0 );

filesize = lseek( fp, 0, SEEK_END );
send( s, &filesize, sizeof(filesize), 0 ); 
lseek(fp, 0, SEEK_SET );

while( total != filesize )
{
sread = read( fp, buf, 100 );
//printf( "file is sending now.. " );
total += sread;
buf[sread] = 0;
send( s, buf, sread, 0 );
}
printf( "File transfer Completed! \n\n" );

close(fp);
close(s);
return 0;
}
