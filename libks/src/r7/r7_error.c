#include <stdarg.h>
#include "r7_zgl.h"

void gl_fatal_error(char *format, ...)
{
  va_list ap;

  va_start(ap,format);

  fprintf(stderr,"TinyGL: fatal error: ");
  vfprintf(stderr,format,ap);
  fprintf(stderr,"\n");
  va_end(ap);

  exit(1);
}
