#include "sl.c.h"
#include <sl/sl.h>
#include <math.h>

SL_API void slu_perspective(float fovy, float aspect, float near, float far)
{
    float xmin, xmax, ymin, ymax;

    ymax = near * (float)tanf(fovy * (float)SL_PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    sl_frustum(xmin, xmax, ymin, ymax, near, far);	
}

SL_API void slu_lookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
    float m[16];
    float x[3], y[3], z[3];
    float mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;

    mag = (float)sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);

    if (mag) 
    {
        z[0] /= mag; 
        z[1] /= mag; 
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = upx; y[1] = upy; y[2] = upz;

    /* X vector = Y cross Z */
    x[0] =  y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] =  y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] =  z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] =  z[0] * x[1] - z[1] * x[0];

    /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */
    mag = (float)sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) 
    {
        x[0] /= mag; 
        x[1] /= mag; 
        x[2] /= mag;
    }

    mag = (float)sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) 
    {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0]; M(0, 1) = x[1]; M(0, 2) = x[2]; M(0, 3) = 0.0f;
    M(1, 0) = y[0]; M(1, 1) = y[1]; M(1, 2) = y[2]; M(1, 3) = 0.0f;
    M(2, 0) = z[0]; M(2, 1) = z[1]; M(2, 2) = z[2]; M(2, 3) = 0.0f;
    M(3, 0) = 0.0f; M(3, 1) = 0.0f; M(3, 2) = 0.0f; M(3, 3) = 1.0f;
#undef M

    sl_mult_matrix(m);
    sl_translate(-eyex, -eyey, -eyez);

    //void gluLookAt(v3<GLdouble> eye, v3<GLdouble> centre, v3<GLdouble> up); 
    //fw = forward = normalize(centre - eye); // subtract then normalize
    //rs = rightside = normalize(forward * up); // cross then normalize
    //up = normalize(rightside * forward); // cross to let vertical, then normalize 
    //lookview_matrix[4][4] = {     
    //    { rs.x, up.x, -fw.x, 0 },     
    //    { rs.y, up.y, -fw.y, 0 },     
    //    { rs.z, up.z, -fw.z, 0 },     
    //    {    0,    0,     0, 1 }} ;

    //translate_matrix[4][4] = {     
    //    { 1, 0, 0, 0 },     
    //    { 0, 1, 0, 0 },     
    //    { 0, 0, 1, 0 },     
    //    { -eye.x, -eye.y, -eye.z, 1 }};

    //matrix[4][4] = lookat_matrix[4][4] = lookview_matrix * translate_matrix; // matrix multiply
}
