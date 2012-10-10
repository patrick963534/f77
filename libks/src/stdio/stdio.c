#include <ks/stdio.h>
#include <stdarg.h>

KS_API FILE* ks_fopen(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

KS_API FILE* ks_freopen(const char *filename, const char *mode, FILE *stream)
{
    return freopen(filename, mode, stream);
}

KS_API FILE* ks_tmpfile(void)
{
    return tmpfile();
}

KS_API void ks_clearerr(FILE *stream)
{
    clearerr(stream);
}

KS_API int ks_fclose(FILE *stream)
{
    return fclose(stream);
}

KS_API int ks_feof(FILE *stream)
{
    return feof(stream);
}

KS_API int ks_ferror(FILE *stream)
{
    return ferror(stream);
}

KS_API int ks_fflush(FILE *stream)
{
    return fflush(stream);
}

KS_API int ks_fgetpos(FILE *stream, fpos_t *pos)
{
    return fgetpos(stream, pos);
}

KS_API size_t ks_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

KS_API int ks_fseek(FILE *stream, long int offset, int whence)
{
    return fseek(stream, offset, whence);
}

KS_API int ks_fsetpos(FILE *stream, const fpos_t *pos)
{
    return fsetpos(stream, pos);
}

KS_API long int ks_ftell(FILE *stream)
{
    return ftell(stream);
}

KS_API size_t ks_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

KS_API int ks_remove(const char *filename)
{
    return remove(filename);
}

KS_API int ks_rename(const char *old_filename, const char *new_filename)
{
    return rename(old_filename, new_filename);
}

KS_API void ks_rewind(FILE *stream)
{
    return rewind(stream);
}

KS_API void ks_setbuf(FILE *stream, char *buffer)
{
    return setbuf(stream, buffer);
}

KS_API int ks_setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
    return setvbuf(stream, buffer, mode, size);
}

KS_API char* ks_tmpnam(char *str)
{
    return tmpnam(str);
}

KS_API int ks_fprintf(FILE *stream, const char *format, ...)
{
    int ret;

    va_list va;
    va_start(va, format);
    ret = vfprintf(stream, format, va);
    va_end(va);

    return ret;
}

KS_API int ks_printf(const char *format, ...)
{
    int ret;

    va_list va;
    va_start(va, format);
    ret = vprintf(format, va);
    va_end(va);

    return ret;
}

KS_API int ks_sprintf(char *str, const char *format, ...)
{
    int ret; 

    va_list va;
    va_start(va, format);
    ret = vsprintf(str, format, va);
    va_end(va);

    return ret;
}

KS_API int ks_vfprintf(FILE *stream, const char *format, va_list arg)
{
    return vfprintf(stream, format, arg);
}

KS_API int ks_vprintf(const char *format, va_list arg)
{
    return vprintf(format, arg);
}

KS_API int ks_vsprintf(char *str, const char *format, va_list arg)
{
    return vsprintf(str, format, arg);
}


KS_API int ks_fscanf(FILE *stream, const char *format, ...)
{
    int ret;

    va_list va;
    va_start(va, format);
    ret = vfscanf(stream, format, va);
    va_end(va);

    return ret;
}

KS_API int ks_scanf(const char *format, ...)
{
    int ret;

    va_list va;
    va_start(va, format);
    ret = vscanf(format, va);
    va_end(va);

    return ret;
}

KS_API int ks_sscanf(const char *str, const char *format, ...)
{
    int ret;

    va_list va;
    va_start(va, format);
    ret = vsscanf(str, format, va);
    va_end(va);

    return ret;
}


KS_API int ks_fgetc(FILE *stream)
{
    return fgetc(stream);
}

KS_API char* ks_fgets(char *str, int n, FILE *stream)
{
    return fgets(str, n, stream);
}

KS_API int ks_fputc(int ch, FILE *stream)
{
    return fputc(ch, stream);
}

KS_API int ks_fputs(const char *str, FILE *stream)
{
    return fputs(str, stream);
}

KS_API int ks_getc(FILE *stream)
{
    return getc(stream);
}

KS_API int ks_getchar()
{
    return getchar();
}

KS_API char* ks_gets(char *str)
{
    return gets(str);
}

KS_API int ks_putc(int ch, FILE *stream)
{
    return putc(ch, stream);
}

KS_API int ks_putchar(int ch)
{
    return putchar(ch);
}

KS_API int ks_puts(const char *str)
{
    return puts(str);
}

KS_API int ks_ungetc(int ch, FILE *stream)
{
    return ungetc(ch, stream);
}

KS_API void ks_perror(const char *str)
{
    return ks_perror(str);
}
