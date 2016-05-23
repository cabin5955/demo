
#ifdef WIN32
#include "cl_win_fun.c"
#endif

enum
{
    WIN_none=-1,
    //-------------------
    WIN_pop,
    //-------------------
    WIN_end
};

typedef void (*CL_FUN_VOID)();
typedef void (*CL_FUN_VOID_INT)(int);
typedef void (*CL_FUN_INT__)(int);
typedef int (*CL_FUN_INT_SCREEN)();
typedef int (*CL_KEY_PRO)(CL_EVENT*);

void cl_set_win_into(CL_FUN_VOID fun,int id);
void cl_set_win_out(CL_FUN_VOID fun,int id);
void cl_set_win_draw_main(CL_FUN_INT_SCREEN fun,int id);
void cl_set_key_pro(CL_KEY_PRO fun,int id);
void cl_set_win_id_clear();
void cl_set_win_id(int id,int layer);
void cl_set_win_layer(int layer,int id);
void cl_set_win_id_back();

#include "cc_win_pop.c"

//-----------------------------------------------------
typedef struct CL_WIN_INIT_FUN_LAYER
{
	CL_FUN_VOID_INT 	cl_win_init;
	int 		layer;
}CL_WIN_INIT_FUN_LAYER;


CL_WIN_INIT_FUN_LAYER cl_win_init_fun_layer[]={
	{cl_do_at_first_pop,0},
};