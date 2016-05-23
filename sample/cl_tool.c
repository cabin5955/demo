S32 cl_scal_xy = 1<<FIX_BIT;
S32 cl_scal_xy_i = 1<<FIX_BIT;

void cl_init_scal(S32 scal)
{    
	cl_scal_xy_i = scal;
	cl_scal_xy=0xffffffff/scal;
}


void cl_set_lcd_main(S32 main_w,S32 main_h)
{
    cl_lcd_main_w=main_w;
    cl_lcd_main_h=main_h;
}

void cl_set_lcd_offset(S32 x,S32 y)
{
	x=(x+1)&0xfffffe;
    cl_lcd_offset_x=x;
    cl_lcd_offset_y=y;
    CL_SCAL(x);
	x=(x+1)&0xfffffe;
    CL_SCAL(y);	    
}

void cl_set_lcd_wh(S32 w,S32 h)
{
    cl_lcd_w=w;
    cl_lcd_h=h;
    cl_lcd_offset_x=0;
    cl_lcd_offset_y=0;
    cl_set_lcd_main(w,h);
}

void cl_free(void **pt)
{
    if(*pt)
    {
        //o_sub_mall_size(*pt);
        free(*pt);
        *pt=0;
    }
}
