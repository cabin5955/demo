// GLSimple2D.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>		// Must have for Windows platform builds
#include "include/GL/glew.h"
#include "include/GL/glut.h"			// Glut (Free-Glut on Windows)

#include <assert.h>
#include "math_3d.h"
#include "texture.h"
#include "pipeline.h"
#include "GLSimple2D.h"
#include "cl_opengl.cpp"

#if 1
int w_window=WIDTH;
int h_window=HEIGHT;
#else
int w_window=1024;
int h_window=768;
#endif

void renderFrame()
{
    static float grey;
    static U32 time[10];
	U32 time_i;
    int inc=0;
    int i,j;
    static int clr=0;
    clr++;
	time_i = cl_get_time_us();
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    //-----------------------------------------------------
	cl_main_draw(event,time);
	
	glFlush(); 
	
	time[0] = cl_get_time_us()-time_i;
}
//-----------------------------------------------------
void RenderSceneCB(int v)
{
	glutTimerFunc(33,RenderSceneCB,0);
	
	glClearColor(0, 0, 0, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
	renderFrame();
}
//-----------------------------------------------------
void display()
{

}
//-----------------------------------------------------
static void InitializeGlutCallbacks()
{
	glutDisplayFunc(display); 				// Tell GLUT to use the method "display" for rendering  
    srand(GetTickCount());
	glutTimerFunc(33,RenderSceneCB,0);
}
//-----------------------------------------------------
int main(int argc, char** argv)
{
	//获得屏幕分辨率
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN); 
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	
	//h_window = 768;w_window = 1212;
	//h_window = 688;w_window = 1024;
	//h_window = 768;w_window = 1024;
	h_window = 320;w_window = 480;
	//h_window = 640;w_window = 960;
	//h_window = 640;w_window = 1136;
	//h_window = 1080;w_window = 1920;
	
    glutInit(&argc, argv); 					// Initialize GLUT  
	glutInitDisplayMode (GLUT_SINGLE); 		// Set up a basic display buffer (only single buffered for now)  

    glutInitWindowSize(w_window, h_window);
    glutInitWindowPosition(nFullWidth - w_window>>1, nFullHeight- h_window-60>>1);
    glutCreateWindow("GLSimple2D");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
    setupGraphics(w_window, h_window);
	cl_init_all();

	glutMouseFunc(mouse_up_down);
	glutMotionFunc(mouse_move);
	glutKeyboardFunc(processNormalKeys);
	InitializeGlutCallbacks();
	
    glutMainLoop();

    return 0;
}
