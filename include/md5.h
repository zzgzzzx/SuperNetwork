/************************/
/*REDIUS�ͻ��˲���ͷ�ļ�*/
/*       2002.12.15     */
/************************/
#include <stdio.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned char *POINTER;
typedef unsigned short int UINT2;
typedef unsigned long  int UINT4;

typedef struct sockaddr_in socketstru;
typedef struct sockaddr socketnew;

/****************/
/* MD5 context. */
/****************/
typedef struct 
{
  UINT4 state[4];					/* state (ABCD) */
  UINT4 count[2];					/* λ����, ģ 2^64 (��λ��ǰ) */
  unsigned char buffer[64];			/* ���뻺���� */
} MD5_CTX;

void MD5Init (MD5_CTX *);
void MD5Update (MD5_CTX *, unsigned char *, unsigned int);
void MD5Final (unsigned char [16], MD5_CTX *);

#ifdef __cplusplus
}
#endif

