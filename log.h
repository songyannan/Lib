#ifndef __SHARE_H__
#define __SHARE_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/***********************MACRO*****************************/
#define   MAX_LOG_SIZE 10<<10 //10Mbyte
/*********************************************************/

/*****************************************************
 *function: read a line date from document
 *para    : buf 
 *          fd
 *return  : success return the count of data(byte)
 *          fail    return -1
 * **************************************************/
int  ReadLine(uint8_t *buf,FILE *fd);
/*******************************************
 *function: read a line date from document
 *para    : 
 *          ->path of log
 *return  : success return  0 
 *          fail    return -1
 *******************************************/
int  LogInit(const char * path);
/*******************************************
 *function: print  message
 *para    : 
 *          ->message of user
 *return  : void
 *******************************************/
void LogMsg(const char *s,...);
/*******************************************
 *function: print error message
 *para    : 
 *          ->message of user
 *return  : void
 *******************************************/
void LogErr(const char *s,...);

#endif
