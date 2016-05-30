#include <iostream>
#include "texture.h"

Texture::Texture(GLenum TextureTarget, const char* fileName)
{
    m_textureTarget = TextureTarget;
    strcpy(m_fileName,fileName);
    m_pImage        = NULL;
}

int Texture::getWidth()
{
		return width;
}

int Texture::getHeight()
{
	return height;
}

GLuint Texture::getTexId()
{
	return m_textureObj;
}

bool Texture::Load()
{

    m_pImage = loadBMP(m_fileName);

	width = m_pImage->width;
	height = m_pImage->height;

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);

	if(m_pImage->format == GL_RGBA){
		glTexImage2D(m_textureTarget,                		//Always GL_TEXTURE_2D
					 0,                            			//0 for now
					 GL_RGBA,                       		//Format OpenGL uses for image
					 m_pImage->width, m_pImage->height,  	//Width and height
					 0,                            			//The border of the image
					 GL_RGBA, 								//GL_RGB, because pixels are stored in RGB format
					 GL_UNSIGNED_BYTE, 						//GL_UNSIGNED_BYTE, because pixels are stored
					                   						//as unsigned numbers
					 m_pImage->pixels);              		//The actual pixel data
	}
	else if(m_pImage->format == GL_RGB){
		glTexImage2D(m_textureTarget, 
					0,
					GL_RGB, 
					width, height, 
					0,
					GL_BGR, 
					GL_UNSIGNED_BYTE, 
					m_pImage->pixels);
	}
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete m_pImage;
    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}


