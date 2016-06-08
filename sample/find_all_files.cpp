
#include <io.h> // _findfirst() _findnext()
#include <string.h> //strcat()
#include <stdio.h>//gets() puts()
#include <direct.h> //_chdir() _getcwd()
#include "imageloader.h"

char comm_path[128]={0};
char all_file_name[1000][80]={0};
int file_num = 0;

char * ResetPath(char *path)//�ж�·���ĺϷ���
{
	int len,i;
	char *temp;//���غϷ�·��
	len=strlen(path);//�����·������
	temp=new char [len];
	
	for(i=0;i<len;i++)
	{
		temp[i]=path[i];
	}
	temp[i]='\0';

	if(len==1)
	{
		strcat(temp,":\\*");
		return temp;
	}
	else if(len==2)
	{
		if(temp[len-1]==':')
		{
			strcat(temp,"\\*");
			return temp;
		}
		else
		{
			temp[1]=':';
			temp[2]='\0';
			strcat(temp,"\\*");
			return temp;
		}
}
else
{
   if(temp[len-1]=='\\')
   {
		strcat(temp,"*");
		return temp;
   }
   else if(temp[len-1]=='*')
   {
    return temp;
   }
   else if(temp[len-1]=='.')
   {
		if(temp[len-2]=='\\')
		{
			temp[len-1]='\0';
			strcat(temp,"*");
			return temp;
		}
		else if(temp[len-2]=='*')
		{
			temp[len-1]='\0';
			return temp;
		}
		else
		{
			temp[len-1]='\0';
			strcat(temp,"\\*");
			return temp;
		}
   }
   else 
   {
		strcat(temp,"\\*");
		return temp;
   }
}
}

bool isBMPFile(char *name)
{
	int len = 0;
	len = strlen(name);
	if(len > 3)
	{
		char file_type[4] = {0};
		strcpy(file_type,&name[len-3]);
		if(strcmp(file_type,"bmp")==0)
			return true;
	}
	return false;
}

void DispFiles(char *Directory)//��ʾָ��Ŀ¼�������ļ����ļ��У���������ʾ���ļ�
{
	long d;
	char temp[80]={0};
	struct _finddata_t data;
	d=_findfirst(Directory,&data);
	if(d==-1)//Ŀ¼������
	{
		printf("Error 1: Invalid Directory...\n");
		return ;
	}
	printf("\n%s֮�µ��ļ�:\n",Directory);

	if(data.attrib!=_A_SYSTEM && isBMPFile(data.name))//��ϵͳ�ļ�
	{
		printf("%s\n",data.name);
		strcpy(temp,comm_path);
		strcat(temp,data.name);
		strcpy(all_file_name[file_num],temp);
		printf("%s\n",all_file_name[file_num]);
		file_num++;
	}
	
	memset(temp,0,sizeof(temp));
	while(!_findnext(d,&data))
	{
		printf("%s\n",data.name);	
		if(data.attrib!=_A_SYSTEM && isBMPFile(data.name))
		{
			strcpy(temp,comm_path);
			strcat(temp,data.name);
			strcpy(all_file_name[file_num],temp);
			printf("%s\n",all_file_name[file_num]);
			file_num++;
			memset(temp,0,sizeof(temp));
		}
	}
	_findclose(d);
}

/* �� s �е��ַ��� s1 �滻�� s2 */

char *strrpl(char *s, const char *s1, const char *s2)
{

	char *ptr;
	while (ptr = strstr(s, s1)) /* �����s���ҵ�s1 */
	{
		memmove(ptr + strlen(s2) , ptr + strlen(s1), strlen(ptr) - strlen(s1) + 1);
		memcpy(ptr, &s2[0], strlen(s2));
	}
	return s;
}

int writeFileName(char* data,int flen){
    FILE *out;
    /**
     * д�ļ�
     */
    if((out=fopen("all_files_name.cpp","a+"))==NULL){
        printf("\nFile open error:3");
        return 3;
    }
    fwrite(data,flen,1,out);/*���ļ���д*/
    fclose(out);
    return 0;
}

void writeAllFileName()
{
	char pre_path[100]={0};
	getcwd(pre_path,sizeof(pre_path));
	char fileName[100] = { 0 };
	char fileNameArr[1000][80] = { 0 };
	for(int i = 0;i < file_num; i++)
	{
		
		memcpy(fileName,all_file_name[i],sizeof(char)*strlen(all_file_name[i]));
		strrpl(fileName,pre_path,"&BMP");
		strrpl(fileName," ","_");
		strrpl(fileName,".","_");
		strrpl(fileName,"\\","_");
		memcpy(fileNameArr[i],fileName,strlen(fileName)*sizeof(char));
		strrpl(fileName,"&BMP","C_Tex_Info XXX");
		strrpl(fileName,"XXX","BMP");
		strcat(fileName,";\n");
		writeFileName(fileName,strlen(fileName)*sizeof(char));
	}

    char fileNameList[]="\nC_Tex_Info *all_files_name[] = \n{\n";
	writeFileName(fileNameList,sizeof(fileNameList));
	for(int i = 0; i < file_num; i++)
	{
		strcat(fileNameArr[i],",\n");
		writeFileName(fileNameArr[i],strlen(fileNameArr[i])*sizeof(char));
	}
	writeFileName("};\n",sizeof("};\n"));
}

void DispSubFiles()//����Ŀ¼��������Ŀ¼
{
	long handle;
	char path[512];
	struct _finddata_t t;
	handle=_findfirst("*",&t);
	if(handle==-1)//���handle=-1,��ʾĿ¼������
	{
	   return ;
	}

	if(t.attrib & _A_SUBDIR)//1. �жϵ�һ���ļ�:���ΪĿ¼
	{
	   if(t.name[0]!='.')//���Ŀ¼�ǿ�
	   {
			_chdir(t.name);
			_getcwd(path,512);
			puts(path);
			DispSubFiles();//�ݹ����
			_chdir("..");//�������ļ��������,�����ϼ�Ŀ¼
	   }
	   //���Ϊ��Ŀ¼,��t.nameΪ"."��"..",�����
	}
	else//�����һ�����ļ�����ΪĿ¼
	{
		 _getcwd(path,512);
		 strcat(path,"\\");
		 strcat(path,t.name);
		 //puts(path);
		 if(t.attrib!=_A_SYSTEM && isBMPFile(t.name))
		 {
			strcpy(all_file_name[file_num],path);
			//printf("%s\n",all_file_name[file_num]);
			file_num++;
		}
	}

	while(!(_findnext(handle,&t)))//2. �ж���һ���ļ�,ֱ�����������ļ�
	{
	   if(t.attrib & _A_SUBDIR)//�����һ���ļ�ΪĿ¼
	   {
			if(t.name[0]!='.')//���Ŀ¼�ǿ�
			{
				 _chdir(t.name);
				 _getcwd(path,512);
				 puts(path);
				 DispSubFiles();//�ݹ����
				 _chdir("..");//�������ļ��������,�����ϼ�Ŀ¼
			}
			//���Ϊ��Ŀ¼,��t.nameΪ"."��"..",�����
		   }
		   else//������ļ�����ΪĿ¼,��ʾ�ļ�����·��
		   {
				_getcwd(path,512);//��ȡ�ļ�·��
				strcat(path,"\\");
				strcat(path,t.name);//�ļ�����·��
				//puts(path);//���·��
				if(t.attrib!=_A_SYSTEM && isBMPFile(t.name))
				{
					strcpy(all_file_name[file_num],path);
					//printf("%s\n",all_file_name[file_num]);
					file_num++;
				}
		   }
	}
	_findclose(handle);
}

void main()
{
	char dirc[512];
	char buf[80]={0};
    
	getcwd(buf, sizeof(buf));
	strcat(buf,"\\xianyou\0");
	int len = strlen(buf);
    printf("current working directory : %s\n", buf);

	char *temp = ResetPath(buf);
	memcpy(comm_path,temp,strlen(temp)-1);
	_chdir(comm_path);
	DispSubFiles();
	int i;
	printf("----------------------------------------------\n");
	for(i=0;i<file_num;i++){
		printf("%s\n",all_file_name[i]);
	}
	//return;
	if(access("bmp.res",0)==0)
	{
		remove("bmp.res");
	}

	if(access("all_files_name.cpp",0) == 0)
	{
		remove("all_files_name.cpp");
	}

	C_BMP_Info bmp_info[1000] = {0};
	
	for(i=0; i < file_num;i++)
	{
		loadBMP(all_file_name[i],&bmp_info[i]);	
	}
	writeAllFileName();

	int info_size = sizeof(C_BMP_Info) * file_num;
	writeFile((char*)bmp_info,info_size);
}