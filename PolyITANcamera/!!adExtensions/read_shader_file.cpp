#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

char* read_shader_file(const char* filename)
{
size_t bytes;
struct stat f_stat;
FILE *fp = NULL;

	if (!filename) return NULL;

	printf("processing shader: %s\n", filename);

	// get file size
	int fh = open(filename, O_RDONLY);
	if(fh >= 0)
	{
		int result = fstat(fh, &f_stat);
		if( result != 0 )
		{
			fprintf( stderr, "An fstat error occurred.\n" );
			return NULL;
		}
		close(fh);
	}
	else
	{
		fprintf(stderr,"Cannot open \"%s\" for read!\n", filename);
		return NULL;
	}

	long size = f_stat.st_size;

	char* buf = new char[size+1];	//plus trailing NULL

	fp = fopen(filename, "r");

	bytes = fread(buf, 1, size, fp);

	buf[bytes] = 0;

	fclose(fp);
	
return buf;
}
