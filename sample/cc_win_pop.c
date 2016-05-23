//--all in this .c end by demo
//==========================================
//--global------------------------------------

//==========================================
//--event & logic------------------------------------
#define $$$$EVENT_LOGIC_STAR_pop$$$$ 
//===========================================================

CL_BMP bg_bmp;
CL_BMP button;
CL_BMP loading_bmp;
CL_BMP *hero_tex[] = 
{
	&BMP_xianyou_image_49_bmpmp,
	&BMP_xianyou_image_51_bmpmp,
	&BMP_xianyou_image_53_bmpmp,
	&BMP_xianyou_image_55_bmpmp,
	&BMP_xianyou_image_57_bmpmp,
	&BMP_xianyou_image_59_bmpmp,
	&BMP_xianyou_image_61_bmpmp,
	&BMP_xianyou_image_63_bmpmp,
	&BMP_xianyou_image_65_bmpmp,
	&BMP_xianyou_image_67_bmpmp,
	&BMP_xianyou_image_69_bmpmp,
	&BMP_xianyou_image_71_bmpmp,
};

CL_BUTTON button_pop[]={
{
	0,2000<<16,
	0,(200<<16),
	BUTTON_0_enable,
	DRAW_C_CT,0,
	{
		&button,
	},
	0
},
};
void cl_one_frame_flash_pop()
{
	
}

void do_init_pop()
{
	cl_init_bmp(&bg_bmp, "xianyou\\bg.bmp");
	cl_init_bmp(&loading_bmp, "xianyou\\loading.bmp");
	cl_init_bmp(&button, "xianyou\\start.bmp");

	button_pop[0].x = 1024<<16;
	button_pop[0].y = 768<<16;
	button_pop[0].x_aim = 1024<<16;
	button_pop[0].y_aim = 768<<16;
}
void do_into_pop()
{
	
}
void do_out_pop()
{
}
void do_the_up_pop(CL_EVENT* k)
{
}
void do_the_down_pop(CL_EVENT* k)
{
}
void do_the_left_pop(CL_EVENT* k)
{
}
void do_the_right_pop(CL_EVENT* k)
{
}

void do_the_mouse_down_pop(int x, int y, int time)
{
}
void do_the_mouse_up_pop(int x, int y, int time)
{
	
}
void do_the_mouse_move_pop(int x, int y, int time)
{
}
int cl_key_pro_pop(CL_EVENT* o_event)
{
    CL_EVENT* our_event;
	int x,y,i=0;
    cl_one_frame_flash_pop();
    //while(i<EVENT_NUM)
    {
        our_event=&o_event[i];
    	x=our_event->x;
    	y=our_event->y;
    	switch(our_event->key)
    	{
        	case OUR_UP:  
        		do_the_up_pop(our_event);
        		break;
        	case OUR_DOWN:  
        		do_the_down_pop(our_event);
        		break;
        	case OUR_LEFT:  
        		do_the_left_pop(our_event);
        		break;
        	case OUR_RIGHT:  
        		do_the_right_pop(our_event);
        		break;
        	case OUR_MOUSE_UP:  
        		do_the_mouse_up_pop(our_event->x,our_event->y,our_event->time);
        		x_0=x;
        		y_0=y;
        		mouse_state=OUR_MOUSE_UP;
        		break; 
        	case OUR_MOUSE_DOWN:  
        		do_the_mouse_down_pop(our_event->x,our_event->y,our_event->time);
        		x_0=x;
        		y_0=y;
        		mouse_state=OUR_MOUSE_DOWN;
        		break; 
        	case OUR_MOUSE_MOVE:
        		if(mouse_state==OUR_MOUSE_DOWN 
        			&& (  abs(x-x_0)>15
        			||    abs(y-y_0)>15 )  )
        		{
        			mouse_state=OUR_MOUSE_MOVE;
        		}
        		if( mouse_state==OUR_MOUSE_MOVE )
        		{
        			do_the_mouse_move_pop(our_event->x,our_event->y,our_event->time);
        		}
        		break;
        	default:
        		break;
    	}
        i++;
        if(our_event->key<=OUR_MOUSE_MOVE)
        {
            our_event->key=OUR_KEY_emty;
        }
    }
	return 0;
}
//==========================================
//--draw------------------------------------
#define $$$$DRAW_STAR_pop$$$$

//=====================================================================

int cl_draw_main_pop()
{
	cl_draw_bmp(&bg_bmp,WIDTH<<15,HEIGHT<<15);
	cl_draw_bmp(&loading_bmp,WIDTH<<15,HEIGHT<<15);
	//cl_draw_main_button(WIN_pop);
	//cl_draw_bmp(&button,1024<<16,768<<16);
	static int hero_run_frame=0;
	//cl_draw_bmp(hero_tex[hero_run_frame++%O_sizeof(hero_tex)]);
	return 0;
}
void cl_do_at_first_pop(int layer)
{
    cl_set_win_draw_main(cl_draw_main_pop,WIN_pop);
    cl_set_key_pro(cl_key_pro_pop,WIN_pop);
    cl_set_win_into(do_into_pop,WIN_pop);
    cl_set_win_out(do_out_pop,WIN_pop);
    cl_set_win_layer(layer,WIN_pop);
    //---add your code here--------------------------
    //cl_set_button(button_pop,O_sizeof(button_pop), WIN_pop);
    do_init_pop();
}


