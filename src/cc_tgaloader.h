//========================================================
/**
*  @file      TGALoader.h
*
*  ��Ŀ������ TGA�ļ�������
*  �ļ�����:  ����ӳ��  
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*
*/     
//========================================================
#ifndef __TGALOADER_H__
#define __TGALOADER_H__

#include "include/GL/glew.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** TGA�ļ������� */
class TGALoader
{
   public:
      
      TGALoader();                            /**< ���캯�� */
      ~TGALoader();
      bool LoadTGA(const char *file);          /**< ����TGA�ļ� */
      void FreeImage();                        /**< �ͷ��ڴ� */ 
      bool Load(const char* fileName); 		   /**< ����TGA�ļ�Ϊ���� */
      unsigned int texture;                        /**< ���������ID�� */
      int imageWidth;                         /**< ͼ���� */
      int imageHeight;                        /**< ͼ��߶� */
      unsigned char *image;                   /**< ָ��ͼ�����ݵ�ָ�� */
      unsigned int type;                      /**< ͼ������GL_RGB ��GL_RGBA */
};
#endif