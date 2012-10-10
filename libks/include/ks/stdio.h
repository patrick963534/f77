#ifndef __KS_STDIO_H__
#define __KS_STDIO_H__

#include <ks/defs.h>
#include <stdio.h>

KS_API FILE*       ks_fopen(const char *filename, const char *mode);
KS_API FILE*       ks_freopen(const char *filename, const char *mode, FILE *stream);
KS_API FILE*       ks_tmpfile(void);

KS_API void        ks_clearerr(FILE *stream);
KS_API int         ks_fclose(FILE *stream);
KS_API int         ks_feof(FILE *stream);
KS_API int         ks_ferror(FILE *stream);
KS_API int         ks_fflush(FILE *stream);
KS_API int         ks_fgetpos(FILE *stream, fpos_t *pos);
KS_API size_t      ks_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
KS_API int         ks_fseek(FILE *stream, long int offset, int whence);
KS_API int         ks_fsetpos(FILE *stream, const fpos_t *pos);
KS_API long int    ks_ftell(FILE *stream);
KS_API size_t      ks_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
KS_API int         ks_remove(const char *filename);
KS_API int         ks_rename(const char *old_filename, const char *new_filename);
KS_API void        ks_rewind(FILE *stream);
KS_API void        ks_setbuf(FILE *stream, char *buffer);
KS_API int         ks_setvbuf(FILE *stream, char *buffer, int mode, size_t size);

KS_API char*       ks_tmpnam(char *str);
KS_API int         ks_fprintf(FILE *stream, const char *format, ...);
KS_API int         ks_printf(const char *format, ...);
KS_API int         ks_sprintf(char *str, const char *format, ...);
KS_API int         ks_vfprintf(FILE *stream, const char *format, va_list arg);
KS_API int         ks_vprintf(const char *format, va_list arg);
KS_API int         ks_vsprintf(char *str, const char *format, va_list arg);

KS_API int         ks_fscanf(FILE *stream, const char *format, ...);
KS_API int         ks_scanf(const char *format, ...);
KS_API int         ks_sscanf(const char *str, const char *format, ...);

KS_API int         ks_fgetc(FILE *stream);
KS_API char*       ks_fgets(char *str, int n, FILE *stream);
KS_API int         ks_fputc(int ch, FILE *stream);
KS_API int         ks_fputs(const char *str, FILE *stream);
KS_API int         ks_getc(FILE *stream);
KS_API int         ks_getchar(void);
KS_API char*       ks_gets(char *str);
KS_API int         ks_putc(int ch, FILE *stream);
KS_API int         ks_putchar(int ch);
KS_API int         ks_puts(const char *str);
KS_API int         ks_ungetc(int ch, FILE *stream);

KS_API void        ks_perror(const char *str);

#endif
