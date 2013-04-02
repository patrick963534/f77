#ifndef __ZMATH__
#define __ZMATH__

#define X v[0]
#define Y v[1]
#define Z v[2]
#define W v[3]

typedef struct M44
{
	float m[4][4];
} M44;

typedef struct M33
{
	float m[3][3];
} M33;

typedef struct M34
{
	 float m[3][4];
} M34;

typedef struct V3
{
	 float v[3];
} V3;

typedef struct V4
{
	 float v[4];
} V4;
	
int  r7_m44_is_identity(const M44* a);
void r7_m44_identity(M44* a);
void r7_m44_copy(M44* a, const M44* b);

void r7_m44_mul_vector4(V4* a, const M44* b, const V4* c);
void r7_m44_inv_ortho(M44* a, M44 b);
void r7_m44_inverse(M44* a, const M44* b);
void r7_m44_multiply(M44* c, const M44* a, const M44* b);
void r7_m44_multiply_left(M44* c, const M44* a);
void r7_m44_transpose(M44* a, const M44* b);
void r7_m44_rotate(M44* c, float t, int u);

void r7_m43_multiply(V3* a, const M44* b, const V3* c);
void r7_m33_multiply(V3* a, const M44* b, const V3* c);

V3   r7_v3_new(float x, float y, float z);
int  r7_v3_normal(V3* a);
void r7_v3_copy(V3* a, const V3* b);

V4 r7_v4_new(float x, float y, float z, float w);

#endif  __ZMATH__
