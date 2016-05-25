/**
*  @file      TGALoader.cpp
*
*  ��Ŀ������ ����ӳ��
*  �ļ�����:  TGA�ļ�����  
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*
*/     
//========================================================
#include "cc_tgaloader.h"
/** ���캯�� */
TGALoader::TGALoader()
{
  /** ����ΪĬ��ֵ */
  image = 0;
  type = 0;
  texture = -1;
  imageWidth = 0;
  imageHeight = 0;
}
/** �������� */
TGALoader::~TGALoader()
{
   FreeImage();           /**< �ͷ��ڴ� */
}
/** ����TGA�ļ� */
bool TGALoader::LoadTGA(const char* file)
{
   FILE *pfile;
   unsigned char tempColor;              /**< ���ڽ�����ɫ���� */
   unsigned char bitCount;               /**< ÿ���ص�bitλ�� */
   int colorMode;                        /**< ��ɫģʽ */
   long tgaSize;                         /**< TGA�ļ���С */
   unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /**< δѹ��TGA�ļ�ͷ */
   unsigned char tgaHeader[12];          /**< �ļ�ͷ */
   unsigned char header[6];              /**< �ļ�ͷǰ6���ֽ� */
   /** ����ļ����Ƿ�Ϊ�� */
   if(!file) 
       return false;
   
   /** ���ļ� */
   pfile = fopen(file, "rb");
   if(!pfile) 
       return false;
   /** ��ȡ�ļ�ͷǰ12���ֽ� */
   fread(tgaHeader, 1, sizeof(tgaHeader), pfile);
   /** �Ƚ��ļ��Ƿ�Ϊδѹ���ļ� */
   if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0)
       {
           printf("�ļ����ʹ���!");
           fclose(pfile);
           return false;
       }
   /** ��ȡ6���ֽ�(�������ȡ��12���ֽں���ȡ6���ֽ�) */
   fread(header, 1, sizeof(header), pfile);
   /** ����ͼ��Ŀ�Ⱥ͸߶� */
   imageWidth = header[1] * 256 + header[0];    
   imageHeight = header[3] * 256 + header[2];   
   /** ��ȡÿ���ص�bitλ�� */
   bitCount = header[4];
   /**��������ɫģʽ��ͼ���С */
   colorMode = bitCount / 8;
   tgaSize = imageWidth * imageHeight * colorMode;
   /** �����ڴ� */
   image = new unsigned char[sizeof(unsigned char) * tgaSize];
   /** ��ȡ���� */
   fread(image, sizeof(unsigned char), tgaSize, pfile);
   /** ��BGA��ʽת��ΪRGA��ʽ */
   for(long index = 0; index < tgaSize; index += colorMode)
       {
           tempColor = image[index];
           image[index] = image[index + 2];
           image[index + 2] = tempColor;
       }
   /** �ر��ļ� */
   fclose(pfile);
   /** ����ͼ������ */
   if(colorMode == 3) 
       type = GL_RGB;
   else 
       type = GL_RGBA;
   return true;
}
/** ����TGA�ļ����������� */
bool TGALoader::Load(const char* fileName)
{
    if(!LoadTGA(fileName))
    {
         printf("����TGA�ļ�ʧ��!");
        exit(0);
    }
    /** ��������������� */
    glGenTextures(1, &texture);
   
    /** ����������� */
    glBindTexture(GL_TEXTURE_2D, texture);
    
    /** �����˲� */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
    /** �������� */
    gluBuild2DMipmaps(GL_TEXTURE_2D, type, imageWidth,
                      imageHeight, type, GL_UNSIGNED_BYTE,
                      image);
   return true;
}
void TGALoader::FreeImage()
{
   /** �ͷ��ڴ� */
   if(image)
      {
         delete[] image;
         image = 0;
      }
}