#define FIX_BIT 16
#define FIX_1   (1<<16)


typedef int INT;
typedef int FIX;
typedef unsigned int    UINT;
typedef unsigned char   UCHAR;
typedef unsigned char   CLR8;
typedef unsigned short  USHORT;
typedef unsigned short  CLR16;
typedef unsigned short  UNICOD;
typedef unsigned int    CLR32;


typedef unsigned long long U64;
typedef unsigned int    U32;
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef long long  S64;
typedef int    S32;
typedef char   S8;
typedef short  S16;

enum
{
    DRAW_NONE=0,
    DRAW_C_C,
    DRAW_C_CT,      //--带通道贴图--
    DRAW_C_CT_TD,   //--带通道贴图，对屏幕半透--
    DRAW_C_CT_TC,   //--带通道贴图，对某个颜色半透--
    DRAW_C_CT_CU,   //--带通道贴图，把自己颜色变亮--
    DRAW_C_T_C,
    DRAW_C_T_C_TD,
    //---------------------------
    DRAW_H=256,
    DRAW_C_C_H,
    DRAW_C_CT_H,      //--带通道贴图--
    DRAW_C_CT_TD_H,   //--带通道贴图，对屏幕半透--
    DRAW_C_CT_TC_H,   //--带通道贴图，对某个颜色半透--
    DRAW_C_CT_CU_H,   //--带通道贴图，把自己颜色变亮--
    DRAW_C_T_C_H,
    DRAW_C_T_C_TD_H,
};

#define CL_ARGB(a,r,g,b) 	(b  + ((g ) << 8) + ((r) << 16)+ ((a) <<24))

#define CL_zero(a) memset(&a,0,sizeof(a))
#define CL_sizeof(a) (sizeof(a)/sizeof(a[0]))
#define CL_SWAP(a,b,t) {t=a; a=b; b=t;}
#define CL_COPY(a,b) {memcpy(&(a),&(b),sizeof(a));}
#define CL_FOR(i,CL_sizeof) for(i=0;i<CL_sizeof;i++)
#define CL_FLOAT_2_P16(u) (int)((u) * 65536.0f)
#define CL_LIMIT(i,i_max,i_min) {\
if(i>i_max) \
    i=i_max; \
else if(i<i_min)\
    i=i_min;\
}

int CL_SCAL_OUT(int a);
int CL_SCAL_I_OUT(int a);
int CL_SCAL_OUT_draw(int a);
int CL_SCAL_BMP_OUT_(int a,int scal);

#define CL_SCAL(a)       a=CL_SCAL_OUT(a)
#define CL_SCAL_I(a)     a=CL_SCAL_I_OUT(a)
#define CL_SCAL_draw(a)  a=CL_SCAL_OUT_draw(a)

#define CL_SCAL_BMP_OUT(a,scal) a=CL_SCAL_BMP_OUT_(a,scal)
#define CL_SCAL_BMP(a,scal) CL_SCAL_BMP_OUT_(a,scal)

typedef struct CL_lib_global
{
    
S32  lcd_offset_x;
S32  lcd_offset_y;
S32  lcd_offset_true_x;
S32  lcd_offset_true_y;
S32  lcd_w;
S32  lcd_h;
S32  lcd_main_w;
S32  lcd_main_h;
S32  lcd_true_w;
S32  lcd_true_h;

S32  clip_x_min;
S32  clip_x_max;
S32  clip_y_min;
S32  clip_y_max;

S32  clip_x_min_bf;
S32  clip_x_max_bf;
S32  clip_y_min_bf;
S32  clip_y_max_bf;
S32  lcd_offset_x_bf;
S32  lcd_offset_y_bf;

} CL_lib_global;

extern CL_lib_global cl_lib_global;

#define cl_lcd_offset_x cl_lib_global.lcd_offset_x
#define cl_lcd_offset_y cl_lib_global.lcd_offset_y
#define cl_lcd_offset_true_x cl_lib_global.lcd_offset_true_x
#define cl_lcd_offset_true_y cl_lib_global.lcd_offset_true_y
#define cl_lcd_w cl_lib_global.lcd_w
#define cl_lcd_h cl_lib_global.lcd_h
#define cl_lcd_main_w cl_lib_global.lcd_main_w
#define cl_lcd_main_h cl_lib_global.lcd_main_h
#define cl_lcd_true_w cl_lib_global.lcd_true_w
#define cl_lcd_true_h cl_lib_global.lcd_true_h

#define cl_clip_x_min cl_lib_global.clip_x_min
#define cl_clip_x_max cl_lib_global.clip_x_max
#define cl_clip_y_min cl_lib_global.clip_y_min
#define cl_clip_y_max cl_lib_global.clip_y_max

#define cl_clip_x_min_bf cl_lib_global.clip_x_min_bf
#define cl_clip_x_max_bf cl_lib_global.clip_x_max_bf
#define cl_clip_y_min_bf cl_lib_global.clip_y_min_bf
#define cl_clip_y_max_bf cl_lib_global.clip_y_max_bf
#define cl_lcd_offset_x_bf cl_lib_global.lcd_offset_x_bf
#define cl_lcd_offset_y_bf cl_lib_global.lcd_offset_y_bf
