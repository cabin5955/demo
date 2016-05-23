
#define  cl_log(...)  printf(__VA_ARGS__)

void checkGlError_line(const char* op,int line,const char* file_name)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        cl_log("after %s() glError (0x%x)-line:%d-%s\n", op, error,line,file_name);
    }
}

#define checkGlError(a) checkGlError_line(a,__LINE__,__FILE__)

char cl_v_shader[] = 
    "\
    attribute vec4 position;\
    attribute vec2 tex_coord_in;\
    varying vec2 tex_coord_out;\
    void main()\
	{\
		gl_Position = position;\
		tex_coord_out = tex_coord_in;\
	}";

char cl_f_shader_td[] = 
	"\
	varying vec2 tex_coord_out;\
	uniform sampler2D texture;\
	uniform vec4 clr;\
	void main()\
	{\
		vec4 c;\
		c = clr*texture2D(texture, tex_coord_out);\
		gl_FragColor = c;\
	}";

CL_GL_FUN cl_gl_fun_clr[6]={0};
	
GLuint cl_loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
	{
        GLint compiled = 0;
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)		//--for err--
		{
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
			{
                char* buf = (char*) malloc(infoLen);
                if (buf)
				{
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    cl_log("Could not compile shader %d:\n%s\n",shaderType, buf);
                    cl_free((void**)&buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader; 
}

GLuint cl_createProgram(const char* pVertexSource, const char* pFragmentSource)
{
	GLuint pixelShader;
	GLuint program;
	char v_src[8<<10],f_src[8<<10];
#ifdef WIN32_
	sprintf(v_src,"%s",pVertexSource);
	sprintf(f_src,"%s",pFragmentSource);
#else
	sprintf(v_src,"precision highp float;%s",pVertexSource);
	sprintf(f_src,"precision highp float;%s",pFragmentSource);
#endif
    GLuint vertexShader = cl_loadShader(GL_VERTEX_SHADER, v_src);
    if (!vertexShader)
	{
        return 0;
    }

    pixelShader = cl_loadShader(GL_FRAGMENT_SHADER, f_src);
    if (!pixelShader)
	{
        return 0;
    }

    program = glCreateProgram();
    if (program)
	{
        GLint linkStatus = GL_FALSE;
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader-v");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader-p");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
		{
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
			{
                char* buf = (char*) malloc(bufLength);
                if (buf)
				{
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    cl_log("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}


int cl_create_shader(CL_GL_FUN*fun,char*v_shader,char*f_shader)
{
	fun->program = cl_createProgram(v_shader, f_shader);
	if (!fun->program)
	{
		cl_log("Could not create program.");
		return 0;
	}
	
	fun->position= glGetAttribLocation(fun->program, "position");
	//glEnableVertexAttribArray(fun->position);
	
	checkGlError("glGetAttribLocation");
	cl_log("glGetAttribLocation(\"position\") = %d\n",
			fun->position);

	fun->tex_coord = glGetAttribLocation(fun->program, "tex_coord_in");
	//glEnableVertexAttribArray(fun->tex_coord);
	
	checkGlError("glGetAttribLocation");
	cl_log("glGetAttribLocation(\"tex_coord_in\") = %d\n",
			fun->tex_coord);
	
	fun->texture= glGetUniformLocation(fun->program, "texture");
	
	glUniform1i(fun->texture, 0);
	
	fun->clr= glGetUniformLocation(fun->program, "clr");
	checkGlError("clr");
	
	return 0;
}

void cl_shader_init()
{
	cl_create_shader(&cl_gl_fun_clr[0],cl_v_shader,cl_f_shader_td);
}

