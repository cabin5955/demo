#ifndef GLSIMPLE2D_H
#define	GLSIMPLE2D_H

#define FIX_1   (1<<16)

typedef unsigned long long U64;
typedef unsigned int    U32;
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef long long  S64;
typedef int    S32;
typedef char   S8;
typedef short  S16;

typedef struct {
    float x,y,z;
	float u,v;
	float u2,v2;
}OF_POINT;

typedef struct CL_BMP
{
    U16 height;
    U16 width;
    U16 x_core;
    U16 y_core;
    U16 w_all;
    U16 h_all;
    U16 bits;
	float x_core_off;
	float y_core_off;
	float w_core_scal;
	float h_core_scal;
    U32 size;
    U32 pos;
	U32	tex_id;
} CL_BMP;

typedef struct 
{
	U32 program;		//--Á¬½ÓµÄ--
	U32 position;	//-arry-the points position--
	U32 tex_coord;	//-arry-index of point--
	U32 tex_coord2;	//-arry-index of point--
	U32 texture;		//-one -index of map--
	U32 texture2;		//-one -index of map--
	U32 clr;			//-one ---
	//GLuint t;			//-one ---
}CL_GL_FUN;

void CompileShaders(GLuint *shaderProgram);
void setupGraphics(int w,int h);
void cl_draw_bmp(CL_BMP *bm,int x,int y);
void cl_init_bmp(CL_BMP *bm,const char *res_path);

#endif