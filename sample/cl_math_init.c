
#define CL_PY2_BIT 12
#define CL_PY2 (1<<CL_PY2_BIT)
#define CL_SIN_BIT 30
#define CL_SIN_1 (1<<CL_SIN_BIT)
#define CL_SIN_BIT_32 (32-CL_SIN_BIT)

void cl_64_time(int a, int b, int *mul_h) 
{
	int c;
	if(1)
		*(mul_h)=((long long)a)*(b)>>32;
	else
	{
		int a_u=((int)a>>16);
		int a_d=a-(a_u<<16);
		int b_u=((int)b>>16);
		int b_d=b-(b_u<<16);
		*(mul_h)=a_u*b_u+((a_d*b_u)+(a_u*b_d)+(a_d>>8)*(b_d>>8)>>16);
	}
}

//------------------------------------------------------
int CL_SCAL_OUT_draw(int a)
{
    int ret;
    ret=(((a>>16)*(cl_scal_xy>>0))+(1<<(FIX_BIT-1))>>FIX_BIT);
    return ret;
}

int CL_SCAL_OUT(int a)
{
    int ret;
    cl_64_time((a<<10)+(1<<9), (cl_scal_xy<<10), &ret);
    ret=ret>>4;
    return ret;
}

int CL_SCAL_I_OUT(int a)
{
    int ret;
    cl_64_time((a<<10)+(1<<9), (cl_scal_xy_i<<10), &ret);
    ret=ret>>4;
    return ret;
}

int CL_SCAL_BMP_OUT_(int a,int scal)
{
    int ret;
    cl_64_time((a<<10)+(1<<9), (scal<<10), &ret);
    ret=ret>>4;
    return ret;
}

//--------------------------------------
S32 sin_cos_S32[CL_PY2/4+1];

void cl_build_sin_cos(void)
{
    S32 tmp;
    S32 ang;
    double theta;
    double py=(3.141592654f)*2/CL_PY2;
    for (ang = 0; ang <= CL_PY2/4; ang++)
    {    
        theta= (double)ang*py;
        tmp=(S32)(sin(theta)*(1<<(CL_SIN_BIT)));
        sin_cos_S32[ang] =tmp;
    }
}

S32 cl_sin_fast(S32 theta)
{
    U32 ang;
    ang=theta+0x40000000;
    
    ang=ang&(0xffffffff>>(32-CL_PY2_BIT));
    if(ang<=(CL_PY2>>2))
        return (sin_cos_S32[ang]);
    else if(ang<(CL_PY2>>1))
        return (sin_cos_S32[(CL_PY2>>1)-ang]);
    else if(ang<(CL_PY2*3>>2))
        return -(sin_cos_S32[ang-(CL_PY2>>1)]);
    else
        return -(sin_cos_S32[CL_PY2-ang]);
}

S32 cl_cos_fast(S32 theta)
{
    U32 ang;
    ang=theta+0x40000000;
    
    ang=ang&(0xffffffff>>(32-CL_PY2_BIT));
    if(ang<=(CL_PY2>>2))
        return (sin_cos_S32[(CL_PY2>>2)-ang]);
    else if(ang<(CL_PY2>>1))
        return -(sin_cos_S32[ang-(CL_PY2>>2)]);
    else if(ang<(CL_PY2*3>>2))
        return -(sin_cos_S32[(CL_PY2*3>>2)-ang]);
    else
        return (sin_cos_S32[ang-(CL_PY2*3>>2)]);
}

S32 cl_find_place_in_increase_mid(S32 ch,S32* table,S32 num)
{
    S32 i;
    S32 min=0;
    S32 max=num;
    S32 cur;
    while(1)
    {
        cur=(max+min)>>1;
        //--------------------------
        if(cur==min)
        {
            return cur;
        }/**/
        //--------------------------
        if(table[cur]>ch)
        {
            max=cur;
        }
        else if(table[cur]<ch)
        {
            min=cur;
        }
        else
        {
            return cur;
        }
    }
}

S32 cl_asin_fast(S32 cos)
{
    return cl_find_place_in_increase_mid(cos,sin_cos_S32,CL_PY2/4);
}

S32 cl_acos_fast(S32 cos)
{
    S32 th;
    th=cl_find_place_in_increase_mid(cos,sin_cos_S32,CL_PY2/4);
    return CL_PY2/4-th;
}
