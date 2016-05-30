#include<time.h>

#include<stdlib.h>

typedef struct 
{    
    int key;
    int x,y;
    int time;
}CL_EVENT;

#define EVENT_NUM 5

enum
{
    OUR_KEY_REALESE=0,
    OUR_KEY_emty=0,
    
    OUR_MOUSE_UP,
    OUR_MOUSE_DOWN,
    OUR_MOUSE_MOVE,
        
    OUR_UP=87,//w
    OUR_DOWN=83,//s
    OUR_LEFT=65,//a
    OUR_RIGHT=68,//d
    O_KEY_BASE=1000,
};

int mouse_state=OUR_MOUSE_UP;
int x_0,y_0;

int cl_time=0;

void cl_set_win_id(int id,int layer);

#include "cl_button.h"
#include "cl_include.c"

CL_BUTTON* cl_button[WIN_end]={0};
int       cl_button_num[WIN_end]={0};

#include "cl_button.c"

CL_FUN_VOID      cl_win_into[WIN_end]={0};
CL_FUN_VOID      cl_win_out[WIN_end]={0};
int win_id[3]={-1,-1,-1};
char win_id_bf[3][20];
int win_id_bf_index=0;
CL_FUN_INT_SCREEN cl_win_draw_main[WIN_end]={0};
CL_KEY_PRO        cl_win_key_pro[WIN_end]={0};
int       		  cl_win_key_disable[WIN_end]={0};
int       		  cl_win_layer[WIN_end]={0};

//-- set win into--
void cl_set_win_into(CL_FUN_VOID fun,int id)
{
    if(id<WIN_end)
    {
        cl_win_into[id]=fun;
    }
}
//-- set win out--
void cl_set_win_out(CL_FUN_VOID fun,int id)
{
    if(id<WIN_end)
    {
        cl_win_out[id]=fun;
    }
}
void cl_set_win_layer(int layer,int id)
{
    if(id<WIN_end)
    {
        cl_win_layer[id]=layer;
    }
}
void cl_set_win_draw_main(CL_FUN_INT_SCREEN fun,int id)
{
    if(id<WIN_end)
    {
        cl_win_draw_main[id]=fun;
    }
}
void cl_set_key_pro(CL_KEY_PRO fun,int id)
{
    if(id<WIN_end)
    {
        cl_win_key_pro[id]=fun;
    }
}
void cl_set_key_disable(int disable,int id)
{
	if(id<WIN_end)
	{
		cl_win_key_disable[id]=disable;
	}
}
void cl_set_win_id_clear()
{
    win_id_bf_index=0;
}
void cl_set_win_id(int id,int layer)
{
    if(id<WIN_end && id!=win_id[layer])
    {
        if(win_id[layer]>=0 && cl_win_out[win_id[layer]])
        {
            cl_win_out[win_id[layer]]();
        }
        if(win_id[layer]>=0 && win_id_bf_index<CL_sizeof(win_id_bf[layer])-1)
        {
            win_id_bf[layer][win_id_bf_index++]=win_id[layer];
        }
        
        win_id[layer]=id;
        if(win_id[layer]>=0 && cl_win_into[id])
        {
            cl_win_into[id]();
        }
    }
}
void cl_set_win_id_back(int layer)
{
    int id;
    if(win_id_bf_index>0)
    {
        win_id_bf_index--;
        id=win_id_bf[layer][win_id_bf_index];
    }
    else
    {
        return;
    }
    if(id<WIN_end)
    {
        if(win_id[layer]>=0 && cl_win_out[win_id[layer]])
        {
            cl_win_out[win_id[layer]]();
        }
        win_id[layer]=id;
    }
}
void cl_win_over(int id)
{
	int layer;
	if(id>=0)
	{
		layer = cl_win_layer[id];
	}
	if(id<WIN_end && id==win_id[layer])
	{
		if(win_id[layer]>=0 && cl_win_out[win_id[layer]])
		{
			cl_win_out[win_id[layer]]();
		}
		win_id[layer]=WIN_none;
	}
}
//================================================================
void cl_do_at_first_all()
{
    int i;
   for(i=0;i<O_sizeof(cl_win_init_fun_layer);i++)
    {
        if(cl_win_init_fun_layer[i].cl_win_init)
        {
            cl_win_init_fun_layer[i].cl_win_init(cl_win_init_fun_layer[i].layer);
        }
    }
}

CL_lib_global cl_lib_global;
extern int mouse_scal;
void cl_main_draw(CL_EVENT *o_event,U32 *info)
{
	static int time=1;
	static int mask=0;
	int i,j,top_id;
	CL_FOR(i,EVENT_NUM)
	{
		if(o_event[i].key != OUR_KEY_emty)
		{
			o_event[i].x = (o_event[i].x*mouse_scal>>16)-cl_lcd_offset_x;//*src_w_i;
			o_event[i].y = (o_event[i].y*mouse_scal>>16)-cl_lcd_offset_y;//*src_h_i;
		}
	}
	
	for(i=O_sizeof(win_id)-1;i>=0;i--)
	{
		if(win_id[i]>=0 && cl_win_key_pro[win_id[i]])
	    {
			cl_key_pro_button(o_event,win_id[i]);
	        cl_win_key_pro[win_id[i]](o_event);
			
			CL_FOR(j,EVENT_NUM)
			{
		        o_event[j].key = OUR_KEY_emty;
			}
	    }
	}
	for(i=O_sizeof(win_id)-1;i>=0;i--)
	{
		if(win_id[i]>=0)
	    {
			top_id = i;
			break;
	    }
	}
	if(top_id>0 && mask<128) 
	{
		mask+=16;
	}
 	else if(top_id==0 && mask>0) 
	{
		mask-=16;
 	}
	
	CL_FOR(i,O_sizeof(win_id))
	{
		if(win_id[i]>=0 && cl_win_draw_main[win_id[i]])
		{
			if(i>0 && i== top_id)
			{
				//cl_mask(0<<16,0<<16,2048<<16,1536<<16,CL_ARGB(mask,0,0,0));
			}
			
	    	cl_win_draw_main[win_id[i]]();

			if(i==0 && top_id==0 && mask>0)
			{
				//cl_mask(0<<16,0<<16,2048<<16,1536<<16,CL_ARGB(mask,0,0,0));
			}
		}
	}
	cl_time++;

	//i=o_get_time_us()/1000;
	
	time=i;
}

void cl_init_win_restart()
{
	CL_zero(win_id);
    cl_set_win_id(WIN_pop,0);
}

void cl_init_all()
{
	
#define MMMMAX 1.0f
extern int mouse_scal;
	int w, h;
	cl_set_win_scal(MMMMAX,MMMMAX);
	w=win_w;
	h=win_h;
	cl_init_scal(mouse_scal);
    cl_set_lcd_wh(w, h);
	cl_build_sin_cos();
    cl_set_lcd_main(WIDTH, HEIGHT);
    cl_set_lcd_offset(w-WIDTH>>1, h-HEIGHT>>1);

	res_init();
    cl_do_at_first_all();
    cl_init_win_restart();
 }
 