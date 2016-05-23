
typedef void (*CL_BUTTON_CB)(struct CL_BUTTON* b);

enum
{
    BUTTON_dispear=-1,
    BUTTON_0_enable=0,
    BUTTON_0_disable,
    BUTTON_1_enable=0,
    BUTTON_1_disable,
    BUTTON_STATUS_NUM,
};

typedef struct CL_BUTTON
{
    S32    x,y;
    S32    x_aim,y_aim;
    S16    status;//--up8 == which bmp   down8== dowm or up 
    S16    bmp_style;//--DRAW_C_CT_TD--
    S16    t;
    CL_BMP* bmp[BUTTON_STATUS_NUM];
    CL_BUTTON_CB fun;
	S32	   move_scal;
} CL_BUTTON;

void cl_set_button(CL_BUTTON* b,int num,int id);
void cl_draw_main_button(int win_id_now);

