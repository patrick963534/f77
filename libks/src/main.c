#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/sys/system.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <string.h>

static GLuint programObject;

GLuint LoadShader(GLenum type, const char *shaderSrc)
{
   GLuint shader;
   GLint  compiled;
   
   shader = glCreateShader(type);

   if ( shader == 0 )
        return 0;

   glShaderSource(shader, 1, &shaderSrc, NULL);
   glCompileShader(shader);

   // Check the compile status
   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   if (!compiled) 
   {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
      
      if (infoLen > 1)
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
         ks_log("Error compiling shader:\n%s\n", infoLog);
         
         free(infoLog);
      }

      glDeleteShader(shader);
      return 0;
   }

   return shader;
}

int Init()
{
   char vShaderStr[] =  
      "attribute vec4 vPosition;    \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "}                            \n";
   
   char fShaderStr[] =  
      "precision mediump float;                     \n"
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);   \n"
      "}                                            \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLint  linked;

   vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
   fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

   programObject = glCreateProgram();
   if (programObject == 0)
      return 0;

   glAttachShader(programObject, vertexShader);
   glAttachShader(programObject, fragmentShader);
   glBindAttribLocation(programObject, 0, "vPosition");
   glLinkProgram(programObject);

   // Check the link status
   glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
   if (!linked) 
   {
      GLint infoLen = 0;

      glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
      
      if (infoLen > 1)
      {
         char* infoLog = malloc(sizeof(char) * infoLen);

         glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
         ks_log("Error linking program:\n%s\n", infoLog);            
         
         free(infoLog);
      }

      glDeleteProgram(programObject);
      return GL_FALSE;
   }

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   return GL_TRUE;
}

void Draw()
{

    GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f, 
                           -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f };

    int width = ks_sys_system_instance()->klass->get_width();
    int height = ks_sys_system_instance()->klass->get_height();
       
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
 
    glUseProgram(programObject);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
 
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ks_sys_system_instance()->klass->flush();
}


int main()
{
    ks_time_t t1, t2;
    ks_container_t* c;
    int ellapse = 0;
    int count = 0;

    ks_log("%d\n", sizeof(long int));

    t1 = ks_time_now();

    c = ks_container_new(sizeof(*c));
    ks_sys_system_init(c);
    ks_sys_system_instance()->klass->create_window("hello", 320, 240);

    Init();

    while (1)
    {
        count++;

        ks_time_sleep(32);

        //Draw();

        t2 = ks_time_now();
        ellapse += ks_time_differ_in_msec(t2, t1);

        if (ellapse > 1000)
        {
            ks_log("%d ", count);
            ellapse = 0;
            count = 0;
        }

        t1 = t2;
    }

    return 0;
}
