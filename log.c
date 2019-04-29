#include "log.h"
#if 0
/***************************************************
 *Function : output data according to the para base
 *Para     : 
 *           ->input nummber
 *           ->base 
 *Return   : void
 ****************************************************/
static void PrintRes(unsigned long num, int base) { 
    /* 递归结束条件 */ 
    if (num == 0) { 
        return;  
    } 
    /* 继续递归 */ 
    PrintRes(num/base, base); 
    /* 逆序打印结果 */ 
    putchar("0123456789abcdef"[num%base]);  
}
/***************************************************
 *Function : output decimal number
 *Para     : 
 *           ->decimal integer nummber
 *Return   : void
 ****************************************************/
static void PrintDec(int dec){
    if (dec < 0){
        putchar('-');
        dec = -dec;
    }
    if(dec == 0){
        putchar('0');
        return;
    }else{
        PrintRes(dec, 10);
    }
}
/***************************************************
 *Function : output octonary number
 *Para     : 
 *           ->decimal integer nummber
 *Return   : void
 ****************************************************/
static void PrintOct(unsigned oct){
    if (oct == 0){
        putchar('0');
        return;
    }else{
        PrintRes(oct,8);
    }
}
/***************************************************
 *Function : output hex number
 *Para     : 
 *           ->decimal integer nummber
 *Return   : void
 ****************************************************/
static void PrintHex(unsigned hex){
    if (hex == 0){
        putchar('0');
        return ;
    }else{
        PrintRes(hex,16);
    }
}
/***************************************************
 *Function : output address 
 *Para     : 
 *           ->decimal integer nummber
 *Return   : void
 ****************************************************/
static void PrintAdd(unsigned long addr){
    putchar('0');
    putchar('x');
    PrintRes(addr, 16);
}
/***************************************************
 *Function : output string
 *Para     : 
 *           ->pointer of string
 *Return   : void
 ****************************************************/
static void PrintStr(char *str){
    int i = 0;

    while (str[i] != '\0')
    {
        putchar(str[i++]);

    }
}
/***************************************************
 *Function : output float
 *Para     : 
 *           ->data
 *Return   : void
 ****************************************************/
static void PrintFlo(double f){
    int temp; 
    /* 先打印整数部分 */ 
    temp = (int)f; 
    PrintRes(temp,10); 
    /* 分隔点 */ 
    putchar('.'); 
    /* 打印小数部分 */ 
    f -= temp; 
    if (f == 0){ 
        /* 浮点型数据至少六位精度 */ 
        for (temp = 0; temp < 6; temp++) { 
            putchar('0');  
        } 
        return;  
    } else { 
        temp = (int)(f*1000000); 
        PrintRes(temp,10);  
    }
}
#endif
/**********************************************
 *function: read a line date from document
 *para    : 
 *          -> buf address 
 *          -> fd
 *return  : success return count of data 
 *          fail    return -1
 **********************************************/
int ReadLine(uint8_t *buf,FILE *fd){                                      
    int count = 0;
    if((NULL != buf) && (fd != NULL)){
        while(!feof(fd)){
            buf[count++] = fgetc(fd);
            if(0x0A == buf[count - 1]){
                buf[count - 1] = '\0';
                break;

            }
        }
    }else{
        return -1;
    }
    return count;
}

/*******************************************
 *function: read a line date from document
 *para    : 
 *          ->path of log
 *return  : success return  0 
 *          fail    return -1
 *******************************************/
int LogInit(const char *path){
    int p_fd;
    struct stat st ;
    stat( path, &st);
    if(st.st_size > MAX_LOG_SIZE){
        char newpath[128];
        char *p = NULL;
        memset(newpath,0,sizeof(newpath));
        sprintf(newpath,"%s",path);
        p = strrchr(newpath,'/');
        memcpy(p+1,".log_dump.txt",13);
        if(rename(path,newpath)){
            perror(path);
            return -1;
        }
    }
    p_fd = open(path,(O_RDWR | O_APPEND | O_CREAT),0666);
    if(p_fd < 0){
        perror("open %s error :");
        return -1;
    }
    fflush(stdout);
    setvbuf(stdout,NULL,_IONBF,0);
    if(dup2(p_fd,STDOUT_FILENO) < 0){
        perror("dup2 log file descripration error:");
        return -1;
    }
    if(dup2(p_fd,STDERR_FILENO) < 0){
        perror("dup2 log file descripration error:");
        return -1;
    }
    close(p_fd);
    return 0;
}
/*******************************************
 *function: print error message
 *para    : 
 *          ->message of user
 *return  : void
 *******************************************/
void LogErr(const char *s,...){
    time_t  t;
    struct  tm *tm;
    memset(&t,0,sizeof(time_t));
    if(time(&t) < 0){
       perror("get time fail");
       return;
    }
    tm = localtime(&t);
    printf("%04d-%02d-%02d %02d:%02d:%02d\t",tm->tm_year+1900, \
           tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    perror(s);
}
/*******************************************
 *function: print  message
 *para    : 
 *          ->message of user
 *return  : void
 *******************************************/
void LogMsg(const char *s,...){
    time_t  t;
    struct  tm *tm;
    memset(&t,0,sizeof(time_t));
    if(time(&t) < 0){
       LogErr("get time fail");
       return;
    }
    tm = localtime(&t);
    printf("%04d-%02d-%02d %02d:%02d:%02d\t%s\n",tm->tm_year+1900, \
           tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,s);
}
#if 0
void LogMsg(char *s,...){
    int i = 0;
    va_list va_ptr;
    va_start(va_ptr, s);
    while (s[i] != '\0'){
        if (s[i] != '%')
        {
            putchar(s[i++]);
            continue;
        }
        switch(s[++i]){
            case 'd': 
                PrintDec(va_arg(va_ptr,int)); 
                break; 
            case 'o': 
                PrintOct(va_arg(va_ptr,unsigned int)); 
                break; 
            case 'x': 
                PrintHex(va_arg(va_ptr,unsigned int)); 
                break; 
            case 'c': 
                putchar(va_arg(va_ptr,int)); 
                break; 
            case 'p': 
                PrintAdd(va_arg(va_ptr,unsigned long)); 
                break; 
            case 'f': 
                PrintFlo(va_arg(va_ptr,double)); 
                break; 
            case 's': 
                PrintStr(va_arg(va_ptr,char *)); 
                break; 
            default : 
                break;
        }
        i++;
    }
    va_end(va_ptr);
}

#endif
int mytrcmp(uint8_t *str1,uint8_t *str2,int len){
    int i = 0;
    //    assert((str1!=NULL) || (str2!=NULL) ||(len <= 0));
    if((NULL != str1) && (NULL != str2)){
        for(i = 0;i < len;i++){
            if(*str1 != *str2)
                return *str1 - *str2;
            str1++;
            str2++;
        }
    }
    return 0;
}


