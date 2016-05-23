#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include "include/GL/glew.h"
#include "image.h"

class Texture
{
public:
    Texture(GLenum TextureTarget, const char* filename);

    bool Load();

    void Bind(GLenum TextureUnit);

	int getWidth();

	int getHeight();

	GLuint getTexId();

private:
    char m_fileName[100];
    GLenum m_textureTarget;
    GLuint m_textureObj;
	Image *m_pImage;
	int width,height;
};


#endif	/* TEXTURE_H */

