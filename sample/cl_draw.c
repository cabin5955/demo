
#define WIDTH 480
#define HEIGHT 320
float win_w_i=2.0f/(WIDTH<<16),win_h_i=2.0f/(HEIGHT<<16);
int win_w=WIDTH,win_h=HEIGHT;
float win_x_offset=0,win_y_offset=0;
float src_w_i,src_h_i;
float src_w,src_h;

void cl_set_win_scal(float w,float h)
{
	src_w = w;
	src_h = h;
	src_w_i = 1.0f/w;
	src_h_i = 1.0f/h;
	win_w_i=w*2.0f/(win_w<<16);
	win_h_i=h*2.0f/(win_h<<16);
	win_x_offset=((float)win_w-WIDTH)/(win_w);
	win_y_offset=-((float)win_h-HEIGHT)/(win_h);
}

GLuint VBO;
GLuint IBO;
//GLuint gWorldLocation;

const GLubyte Indices[] =
{
     0, 1, 2,
     2, 3, 0
};
const GLubyte Indices_tra[] =
{
     0, 1, 2,
};

GLuint indexBuffer= NULL;
GLuint indexBuffer_tra= NULL;
GLuint gSampler;

void o_set_point(int x_pos,int y_pos,
                 int w_scal,int h_scal,
                 int x_start,int y_start,
                 int x_anchor,int y_anchor,
                 int w_range,int h_range,
                 int h_turn,
                 OF_POINT *point,
                 CL_BMP* bmp,
                 CL_BMP* bmp2)
{
	float x,y,w,h;
	float x_off,y_off,w_sub,h_sub;
	float xstart,ystart,wrange,hrange;
	float xstart2=0,ystart2=0,wrange2,hrange2;
    float temp;
	int x_d_old=x_pos,y_d_old=y_pos;
	
	float w_win,h_win;
	w_win=win_w_i;
	h_win=win_h_i;
	
	if(w_scal==0 || h_scal==0 )
	{
		w_scal = w_range*bmp->w_all;
		h_scal = h_range*bmp->h_all;
	}
	else
	{
		w_scal = w_scal*bmp->w_all;
		h_scal = h_scal*bmp->h_all;
	}
	
	xstart= x_start/65536.0f;
	ystart= y_start/65536.0f;
	wrange= w_range/65536.0f;
	hrange= h_range/65536.0f;
	wrange+=xstart;
	hrange+=ystart;
	w_scal +=x_pos;
	h_scal +=y_pos;

	xstart = xstart*bmp->w_core_scal + bmp->x_core_off;//--get the new x_off--
	wrange = wrange*bmp->w_core_scal + bmp->x_core_off;//--get the new wrange--
	ystart = ystart*bmp->h_core_scal + bmp->y_core_off;//--get the new y_off--
	hrange = hrange*bmp->h_core_scal + bmp->y_core_off;//--get the new ystart--
	
	if(h_turn)
	{
		temp = wrange;
		wrange = xstart;
		xstart = temp;
	}
	if(xstart<0)
	{
		x_pos -= xstart*(bmp->width<<16);
		xstart = 0;
	}
	if(xstart>1.0f)
	{
		x_pos +=(xstart-1.0f)*(bmp->width<<16);
		xstart = 1.0f;
	}
	if(wrange<0)
	{
		w_scal += wrange*(bmp->width<<16);
		wrange = 0;
	}
	if(wrange>1.0f)
	{
		w_scal -=(wrange-1.0f)*(bmp->width<<16);
		wrange = 1.0f;
	}
	if(ystart<0)
	{
		y_pos -= ystart*(bmp->height<<16);
		ystart = 0;
	}
	if(hrange>1.0f)
	{
		h_scal -=(hrange-1.0f)*(bmp->height<<16);
		hrange = 1.0f;
	}
	//----------------------------------
	x_anchor -= x_start;
	y_anchor -= y_start;
	x_pos -= x_anchor*bmp->w_all;
	y_pos -= y_anchor*bmp->h_all;
	w_scal -= x_anchor*bmp->w_all;
	h_scal -= y_anchor*bmp->h_all;

	xstart2 = (x_pos-x_d_old)/((float)(bmp2->width<<16));
	wrange2 = (w_scal-x_d_old)/((float)(bmp2->width<<16));
	ystart2 = (y_pos-y_d_old)/((float)(bmp2->height<<16));
	hrange2 = (h_scal-y_d_old)/((float)(bmp2->height<<16));
	
	x = x_pos*w_win - 1.0f;
	y = 1.0f - y_pos*h_win;
	w = w_scal*w_win - 1.0f;
	h = 1.0f - h_scal*h_win;
	
	point[0].x=x;
	point[0].y=y;
	point[0].u=xstart;
	point[0].u2=xstart2;
	point[0].v=hrange;//ystart;
	point[0].v2=ystart2;
	
	point[1].x=w;
	point[1].y=y;
	point[1].u=wrange;
	point[1].u2=wrange2;
	point[1].v=hrange;//ystart;
	point[1].v2=ystart2;
	
	point[2].x=w;
	point[2].y=h;
	point[2].u=wrange;
	point[2].u2=wrange2;
	point[2].v=ystart;//hrange;
	point[2].v2=hrange2;
	
	point[3].x=x;
	point[3].y=h;
	point[3].u=xstart;
	point[3].u2=xstart2;
	point[3].v=ystart;//hrange;
	point[3].v2=hrange2;
}

void cl_draw_bmp(CL_BMP *bmp,int x,int y)
{
    Pipeline p;
	//static float Scale = 0.0f;
    //Scale += 0.001f;
    //p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    //p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    //p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

    //glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

	OF_POINT point[4];
	OF_POINT *pp = point;

	o_set_point(x,y,	
				FIX_1,FIX_1,
                0,0,
                FIX_1/2,FIX_1/2,
                FIX_1,FIX_1,
				0,
				pp,bmp,bmp);
				
	float tt=1.0f;//t/256.0f;
	glUseProgram(cl_gl_fun_clr[0].program);
	checkGlError("glUseProgram");
	glUniform4f(cl_gl_fun_clr[0].clr, tt,tt,tt,1.0f);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bmp->tex_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
		sizeof(OF_POINT), (float*)&pp->x);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
		sizeof(OF_POINT), (float*)&pp->u);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	
	glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), 
					GL_UNSIGNED_BYTE, 0);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    
}

//====================================================================================
void cl_draw_bmp_clr(int x_d,int y_d,
	int w_d,int h_d,
	int x_s,int y_s,
	int x_0,int y_0,
	int w_s,int h_s,
	int h,
	CL_BMP* b_s,
	CL_GL_FUN* gl_fun)
{
	if(b_s==0 || b_s->width==0||b_s->height==0)
	{
		return;
	}
	//--
	OF_POINT point[4];

	o_set_point(x_d,y_d,w_d,h_d,x_s,y_s,x_0,y_0,w_s,h_s,h,point,b_s,b_s);
	
	if(point[0].x<-1.0f && point[1].x<-1.0f 
		&&point[2].x<-1.0f && point[3].x<-1.0f)
	{
		return;
	}
	if(point[0].x>1.0f && point[1].x>1.0f 
		&&point[2].x>1.0f && point[3].x>1.0f)
	{
		return;
	}
	//-------------------------------------------
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D, b_s->tex_id);
	//-------------------------------------------
	glEnableVertexAttribArray(gl_fun->position);
	glEnableVertexAttribArray(gl_fun->tex_coord);
	
	glVertexAttribPointer(gl_fun->tex_coord, 2, GL_FLOAT, GL_FALSE, 
		sizeof(OF_POINT), (float*)&point->u);
	//--指定数组到变量--
	glVertexAttribPointer(gl_fun->position, 2, GL_FLOAT, GL_FALSE, 
		sizeof(OF_POINT), (float*)&point->x);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	
	glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), 
					GL_UNSIGNED_BYTE, 0);
					
	glDisableVertexAttribArray(gl_fun->position);
	glDisableVertexAttribArray(gl_fun->tex_coord);
}

void cl_bmp_bilt_c_ct_cu_0( FIX x_d,FIX y_d,
	FIX x_s,FIX y_s,
	FIX x_0,FIX y_0,
	FIX w_s,FIX h_s,
	int t,
	CL_BMP* b_s)
{
	float tt=t/256.0f;

	glUseProgram(cl_gl_fun_clr[0].program);
	checkGlError("glUseProgram");
	glUniform4f(cl_gl_fun_clr[0].clr, tt,tt,tt,1.0f);
	cl_draw_bmp_clr(x_d,y_d,0,0,x_s,y_s,x_0,y_0,w_s,h_s,0,b_s,&cl_gl_fun_clr[0]);
}


void cl_bmp_bilt_c_ct_tc_0( FIX x_d,FIX y_d,
	FIX x_s,FIX y_s,
	FIX x_0,FIX y_0,
	FIX w_s,FIX h_s,
	UINT c,
	CL_BMP* b_s)
{
	float a,r,g,b;
	a = ((c>>24)&0xff)/64.0f;
	r = ((c>>0)&0xff)/64.0f;
	g = ((c>>8)&0xff)/64.0f;
	b = ((c>>16)&0xff)/64.0f;
	glUseProgram(cl_gl_fun_clr[0].program);
	checkGlError("glUseProgram");
	glUniform4f(cl_gl_fun_clr[0].clr, r,g,b,a);
	cl_draw_bmp_clr(x_d,y_d,0,0,x_s,y_s,x_0,y_0,w_s,h_s,0,b_s,&cl_gl_fun_clr[0]);
}
