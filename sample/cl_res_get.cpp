#define O_sizeof(a) (sizeof(a)/sizeof(a[0]))

#include "all_files_name.cpp"

#define  FILE_NUM  (sizeof(all_files_name)/sizeof(all_files_name[0]))

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGBA,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGBA, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureId; //Returns the id of the texture
}

void cl_init_bmp(CL_BMP *bmp,const char *res_path)
{
	Texture* pTexture = NULL;
	pTexture = new Texture(GL_TEXTURE_2D, res_path);
	if (!pTexture->Load()) {
        return ;
    }
	bmp->tex_id = pTexture->getTexId();
	bmp->width = pTexture->getWidth();
	bmp->height= pTexture->getHeight();
	bmp->w_all = pTexture->getWidth();
	bmp->h_all = pTexture->getHeight();
	bmp->x_core= 0;
	bmp->y_core= 0;
	bmp->bits  = 5;
	bmp->x_core_off = 0.0f;
	bmp->y_core_off = 0.0f;
	bmp->w_core_scal= 1.0f;
	bmp->h_core_scal= 1.0;
	bmp->pos = 0;
	bmp->size= 0;
}

void res_init()
{
	C_BMP_RES bmp_info[FILE_NUM] = {0};
	int info_size = sizeof(C_BMP_RES)*FILE_NUM;

	FILE* f;
    if((f=fopen("bmp.res","rb"))==NULL){
        printf("\nFile open error:3");
        return ;
    }
	
	fseek(f,-info_size,SEEK_END);
    fread(bmp_info,info_size,1,f);

	fseek(f,0,SEEK_SET);
	for(int i = 0; i < FILE_NUM; i++)
	{
		C_CLR_RGBA *imgData = (C_CLR_RGBA*)malloc(bmp_info[i].size);
		memset(imgData,0,bmp_info[i].size);
		fread(imgData,bmp_info[i].size,1,f);

		Image* image = new Image((char*)imgData, bmp_info[i].width, bmp_info[i].height,GL_RGBA);
		all_files_name[i]->width = bmp_info[i].width;
		all_files_name[i]->height= bmp_info[i].height;
		all_files_name[i]->w_all = bmp_info[i].width;
		all_files_name[i]->h_all = bmp_info[i].height;
		all_files_name[i]->x_core= 0;
		all_files_name[i]->y_core= 0;
		all_files_name[i]->bits  = 5;
		all_files_name[i]->x_core_off = 0.0f;
		all_files_name[i]->y_core_off = 0.0f;
		all_files_name[i]->w_core_scal= 1.0f;
		all_files_name[i]->h_core_scal= 1.0;
		all_files_name[i]->pos = 0;
		all_files_name[i]->size= bmp_info[i].size;
		all_files_name[i]->tex_id = loadTexture(image);

		delete image;
	}

	fclose(f);	
}

