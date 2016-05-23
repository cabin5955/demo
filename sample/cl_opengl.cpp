
#include "cl_define.h"
#include "cl_tool.c"
#include "cl_shader.c"
#include "cl_res_get.cpp"
#include "cl_math_init.c"
#include "cl_draw.c"
#include "cl_main.c"

//GLuint ShaderProgram;
CL_EVENT event[EVENT_NUM]={0};
int mouse_s=GLUT_UP;
int mouse_scal=1<<16;

void setupGraphics(int w,int h)
{
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
	if(w*2>=h*3)//-->3:2
	{
		//win_h=1366;
		//win_w = (win_h)*w/h;
	}
	mouse_scal = (win_h<<16)/h;
	/*
	CompileShaders(&ShaderProgram);
	glUseProgram(ShaderProgram);
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWorldLocation != 0xFFFFFFFF);
	
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	glUniform1i(gSampler, 0);
	*/
	cl_shader_init();
	glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glGenBuffers(1, &indexBuffer_tra);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_tra);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_tra), Indices_tra, GL_STATIC_DRAW);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glDisable(GL_DEPTH_TEST); // 关闭深度测试
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA); //基于源象素alpha通道值的半透明混合函数
}
//======================================================================
void mouse_up_down(int button, int state,int x, int y)
{
	mouse_s = state;
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_UP)
		{
			event[0].x=x;
			event[0].y=y;
			event[0].key=OUR_MOUSE_UP;
		}
 		else if(state == GLUT_DOWN)
		{
			event[0].x=x;
			event[0].y=y;
			event[0].key=OUR_MOUSE_DOWN;
		}
	}
}
void mouse_move(int x, int y)
{
	if(event[0].key==OUR_KEY_emty)
	{
		event[0].x=x;
		event[0].y=y;
		event[0].key=OUR_MOUSE_MOVE;
	}
}

void processNormalKeys(unsigned char key,int x,int y)
{
	if(key==27)
		exit(0);
	else
	{
		event[1].x=x;
		event[1].y=y;
		event[1].key=O_KEY_BASE+key;
		
		cl_log("processNormalKeys get key -%c-\n",key);
	}
}
