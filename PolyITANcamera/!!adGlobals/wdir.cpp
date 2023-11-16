#include "wdir.h"
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/auxv.h>

//CString projDir;
//
//CString getProjectDir()
//{
//const DWORD count=600;
//char *exepath;
//char *path;
//int i;
//CString subtle_dir;
//
//	if (projDir.GetLength() > 0) return projDir;
//
//	exepath = new char[count];
//	path    = new char[count];
//
//	GetModuleFileName(GetModuleHandle(NULL), exepath, count);  // handle to module
//
//	for (i=lstrlen(exepath)-1;i>=0;i--)
//	{
//		if (exepath[i]=='\\')
//		{
//			strncpy(path,exepath,i+1);
//			path[i+1]=0;				//add terminator
//			break;
//		}
//	}
//
//delete [] exepath;
////subtle_dir = CString(path);
////delete [] path;
////return subtle_dir;
//projDir = CString(path);
//return projDir;
//}

typedef char TCHAR;

TCHAR dir_project[PATH_MAX] = "";

TCHAR* getProjectDir()
{
TCHAR exepath[PATH_MAX];

int i;

	if (strlen(dir_project) == 0)
	{
		strcpy(exepath, (char*)getauxval(AT_EXECFN));

		for (i=strlen(exepath)-1; i>=0; i--)
		{
			if (exepath[i]=='/')
			{
				exepath[i+1]=0;				//place terminator
				break;
			}
		}
		sprintf(dir_project, "%s", exepath);		
	}

return dir_project;
}


char* FullPathToFile(const char* fileName)
{
	static char strProjDir[PATH_MAX];

	strcpy(strProjDir, getProjectDir());
	return strcat(strProjDir, fileName);
}


char* GetFileExtension(const char* fileName)
{
	static char strFileExt[12];

	const char* extPos = strrchr(fileName, '.');
	if (extPos == NULL) return NULL;

	return strcpy(strFileExt, extPos);
}
