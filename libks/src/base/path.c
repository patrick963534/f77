#include <ks/path.h>
#include <ks/libc.h>
#include <string.h>

#define Is_Seperator(p)     ((p) == '\\' || (p) == '/')

KS_API char* ks_path_basename(char* buf, int sz, const char* file)
{
    const char *p, *s;
    char* k;
    int i = sz - 1;

    s = file;
    for (p = file; *p != 0; p++)
    {
        if (Is_Seperator(*p))
            s = p;
    }

    if (s != file)
        s++;

    k = buf;
    p = s;

    while (*p != 0 && --i > 0)
        *k++ = *p++;

    *k = '\0';

    return buf;
}

KS_API char* ks_path_folder(char* buf, int sz, const char* file)
{
    const char *p, *e;
    char* k;
    int i = sz - 1;

    e = file;
    for (p = file; *p != 0; p++)
    {
        if (Is_Seperator(*p))
            e = p;
    }

    p = file;
    k = buf;

    while (p < e && --i > 0)
        *k++ = *p++;

    *k = '\0';

    return buf;
}

KS_API char* ks_path_extention(char* buf, int sz, const char* file)
{
    const char *p, *s;
    char* k;
    int i = sz - 1;

    s = file;
    for (p = file; *p != 0; p++)
    {
        if (Is_Seperator(*p))
            s = p;
    }

    for (p = s, s = NULL; *p != 0; p++)
        if (*p == '.')
            s = p;

    k = buf;
    p = s;

    while (p && p < file && --i > 0)
        *k++ = *p++;

    *k = '\0';

    return buf;
}

KS_API char* ks_path_without_ext(char* buf, int sz, const char* file)
{
    const char *p, *e;
    char* k;
    int i = sz - 1;

    e = file;
    for (p = file; *p != 0; p++)
    {
        if (Is_Seperator(*p))
            e = p;
    }

    for (p = e, e = NULL; *p != 0; p++)
        if (*p == '.')
            e = p;

    if (e == NULL)
        e = p;

    k = buf;
    p = file;

    while (p < e && --i > 0)
        *k++ = *p++;

    *k = '\0';

    return buf;
}

KS_API char* ks_path_join(char* buf, int sz, const char* folder, const char* filename)
{
    const char *p;
    char* k;
    int i = sz - 1;

    k = buf;
    p = folder;

    for (k = buf, p = folder; *p != 0 && i > 0; k++, p++, i--)
        *k = *p;

    for (k--, i++; !Is_Seperator(*k) && i < sz; k--, i++);

    if (i > 0)
    {
        *k++ = '/';
        i--;
    }    

    for (p = filename; Is_Seperator(*p); p++);

    while (*p != 0 && --i > 0)
        *k++ = *p++;

    *k = '\0';

    return buf;
}
