#ifndef _LIBC_STDIO_H
#define _LIBC_STDIO_H

#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define _IOFBF 0 /* setvbuf should set fully buffered */
#define _IOLBF 1 /* setvbuf should set line buffered */
#define _IONBF 2 /* setvbuf should set unbuffered */
#define BUFSIZ 1024 /* size of buffer used by setbuf */
#define EOF (-1)
#define UNGOTC_EMPTY (-0x400)

struct __fbuf {
    char* base;
    char* ptr; /* current pointer */
    size_t size;
};
typedef struct __fbuf __fbuf_t;

struct __rwbuf {
    __fbuf_t rbuf;
    __fbuf_t wbuf;
    char* base;
    size_t size;
};
typedef struct __rwbuf __rwbuf_t;

struct __file {
    int _flags; /* flags, below; this FILE is free if 0 */
    int _file; /* fileno, if Unix descriptor, else -1 */
    size_t _r; /* read space left */
    size_t _w; /* write space left */
    __rwbuf_t _bf; /* rw buffer */
    int _ungotc; /* ungot char. If spot is empty, it equals to UNGOTC_EMPTY */
};
typedef struct __file FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

FILE* fopen(const char* filename, const char* mode);
int fclose(FILE* stream);

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int fseek(FILE* stream, uint32_t offset, int whence);
int fflush(FILE* stream);

int fputc(int c, FILE* stream);
int putc(int c, FILE* stream);
int putchar(int c);

int fputs(const char* s, FILE* stream);
int puts(const char* s);

int fgetc(FILE* stream);
int getc(FILE* stream);
int getchar();
int ungetc(int c, FILE* stream);

char* fgets(char* s, int size, FILE* stream);

int setvbuf(FILE* stream, char* buf, int mode, size_t size);
void setbuf(FILE* stream, char* buf);
void setlinebuf(FILE* stream);

int vsnprintf(char* s, size_t n, const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);
int snprintf(char* s, size_t n, const char* format, ...);
int sprintf(char* s, const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);

int vsscanf(const char* buf, const char* format, va_list arg);
int sscanf(const char* str, const char* fmt, ...);
int scanf(const char* fmt, ...);

__END_DECLS

#endif