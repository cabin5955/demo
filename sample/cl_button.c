

void cl_set_button(CL_BUTTON* b,int num,int id)
{
    if(id<WIN_end)
    {
        cl_button[id]=b;
        cl_button_num[id]=num;
    }
}

int do_the_mouse_down_button(int x, int y, int time,int win_id_top)
{
	CL_BUTTON* b = cl_button[win_id_top];
	int b_num = cl_button_num[win_id_top];
	int i,status;
	CL_FOR(i,b_num)
	{
		status = b[i].status&0xff;
		if(abs(x-(b[i].x>>16)) < b[i].bmp[status]->w_all/2
		 && abs(y-(b[i].y>>16)) < b[i].bmp[status]->h_all/2)
		{
			b[i].status |=0x100;
			return 1;
		}
	}
	return 0;
}
int do_the_mouse_up_button(int x, int y, int time,int win_id_top)
{
	CL_BUTTON* b = cl_button[win_id_top];
	int b_num = cl_button_num[win_id_top];
	int i,status;
	CL_FOR(i,b_num)
	{
		status = b[i].status&0xff;
		if(b[i].bmp[status]
		 && abs(x-(b[i].x>>16)) < b[i].bmp[status]->w_all/2
		 && abs(y-(b[i].y>>16)) < b[i].bmp[status]->h_all/2
		 && (b[i].status &0x100))
		{
			if(b[i].fun)
			{
				b[i].fun(&b[i]);
			}
			b[i].status &=0xff;
			return 1;
		}
		b[i].status &=0xff;
	}
	return 0;
}
void do_the_mouse_move_button(int x, int y, int time,int win_id_top)
{
}
void cl_button_updata(int win_id_top)
{
	CL_BUTTON* b = cl_button[win_id_top];
	int b_num = cl_button_num[win_id_top];
	int i,status;
	CL_FOR(i,b_num)
	{
		if(b[i].move_scal)
		{
			b[i].x += (b[i].x_aim - b[i].x)*b[i].move_scal>>8;
			b[i].y += (b[i].y_aim - b[i].y)*b[i].move_scal>>8;
		}
	}
}
int cl_key_pro_button(CL_EVENT* o_event,int win_id_top)
{
    CL_EVENT* our_event;
	int x,y,i=0;
	cl_button_updata(win_id_top);
    while(i<EVENT_NUM)
    {
        our_event=&o_event[i];
    	x=our_event->x;
    	y=our_event->y;
    	switch(our_event->key)
    	{
        	case OUR_MOUSE_UP:  
        		if(do_the_mouse_up_button(our_event->x,our_event->y,our_event->time,win_id_top))
        		{
            		our_event->key=OUR_KEY_emty;
        		}
        		break; 
        	case OUR_MOUSE_DOWN:
        		if(do_the_mouse_down_button(our_event->x,our_event->y,our_event->time,win_id_top))
        		{
            		our_event->key=OUR_KEY_emty;
        		}
        		break; 
        	case OUR_MOUSE_MOVE:
        		if( mouse_state==OUR_MOUSE_MOVE )
        		{
        			do_the_mouse_move_button(our_event->x,our_event->y,our_event->time,win_id_top);
        		}
        		break;
        	default:
        		break;
    	}
        i++;
    }
	return 0;
}
void cl_draw_main_button(int win_id_now)
{
	CL_BUTTON* b = cl_button[win_id_now];
	int b_num = cl_button_num[win_id_now];
	int i,status;
	cl_set_win_scal(1.0f,1.0f);
	CL_FOR(i,b_num)
	{
		cl_bmp_bilt_c_ct_tc_0(b[i].x,b[i].y,
						0,0,
						FIX_1/2,FIX_1/2,
						FIX_1,FIX_1,
						CL_ARGB(32,64,64,64),
						b[i].bmp[0]);
		/*
		status = b[i].status & 0xff;
		if(b[i].status & 0x100)
		{
			cl_bmp_bilt_c_ct_tc_0(b[i].x,b[i].y,
						0,0,
						FIX_1/2,FIX_1/2,
						FIX_1,FIX_1,
						CL_ARGB(32,64,64,64),
						b[i].bmp[status]);
		}
		else
		{
			cl_bmp_bilt_c_ct_0(b[i].x,b[i].y,
						0,0,
						FIX_1/2,FIX_1/2,
						FIX_1,FIX_1,
						b[i].bmp[status]);
		}
		*/
	}
}

