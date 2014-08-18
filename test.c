#include <dirent.h> 
#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <ftw.h>

void parsedir(char *dir_name);
int ftwfunc(const char *path, const struct stat *sptr, int type, struct FTW *ftwflag);

int main(void)
{
	/*
	DIR     *d;
	struct 	dirent *dir;
	struct	stat *fstat;
	char	dirname[255];
	int 	i;
	*/
	
	nftw(".", ftwfunc, 1, FTW_DEPTH);
	
	/* parsedir("."); */
		
	/*
	d = opendir(".");
	
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type == DT_DIR) {
				strcpy(dirname, dir->d_name);
				i = 0;
				while (dirname[i] != '\0') {
					dirname[i] = toupper((int)dirname[i]);
					i++;
				}
				printf("[%s]\n", dirname);
			}
			else {
				stat(dir->d_name, fstat);
				printf("%-20s %20d \n", dir->d_name, (int)fstat->st_size);
			}
		}	
	}
	
	closedir(d);
	*/
	
	return(0);
}

void parsedir(char *dir_name) {
	DIR     *d;
	struct 	dirent *dir;
	struct	stat *fstat;
	
	printf("Running parsedir on %s\n", dir_name);
	d = opendir(dir_name);
	
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type == DT_DIR) {
				printf("[%s]\n", dir->d_name);
				if ( (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0) ) {
					parsedir(dir->d_name);
				}
			}
			else {
				stat(dir->d_name, fstat);
				printf("%-50s %15d \n", dir->d_name, (int)fstat->st_size);
			}
		}	
	}
}

int ftwfunc(const char *path, const struct stat *sptr, int type, struct FTW *ftwflag ) {
	if (type == FTW_D) {
		printf("[%s]\n", path);
	}
	
	if (type == FTW_F) {
		printf("%s \n", path);
	}
	
	return 0;
}
