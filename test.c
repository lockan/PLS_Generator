#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> 
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#define pls_header "[playlist]";
#define mp3headersize 

struct songEntry {
	char 	fileName	[256];	/* get from struct stat */
	char 	songArtist	[256];	/* get by reading from id3 */
	char 	songTitle	[256];	/* get by reading from id3 */
	int		songLength;			/* get filesize from struct stat, calc length */
};

void plsFromDir(char *dir_name);
void getTitleName(FILE *fp, char *tnBuffer);	/* get song title from id3 */
void getArtistName(FILE *fp, char *anBuffer);	/* get artist from id3 */
int calcSongLength(int size_bytes, int bitrate); /* calc using filesize, bitrate */
int getFileSize(FILE *fp);						/* get from struct stat */
int calculateBitRate(FILE *fp);					/* calc data from bits id3 header block */


int main(int argc, char *argv[])
{	
	plsFromDir(".");
	
	return(0);
}

void plsFromDir(char *dir_name) {
	int 	numEntries;		/* Number of songs in single playlist.  */
	DIR     *dir;
	struct 	dirent *dentry;
	struct	stat *dstat;
	struct 	songEntry *song;
	FILE	*songfile;
	int		song_size;
	int		song_bitrate;
	
	printf("Generating PLS from files in directory \" %s \" \n", dir_name);
	
	dir = opendir(dir_name);
	if (dir) {
		while ((dentry = readdir(dir)) != NULL) {
			if (dentry->d_type == DT_REG) {
				if ((song = malloc(sizeof(struct songEntry))) != NULL) {
					if ((dstat = malloc(sizeof(struct stat))) != NULL) {
						stat(dentry->d_name, dstat);
						/* printf("%-50s %15d \n", dentry->d_name, (int)dstat->st_size); */
						
						songfile = fopen(dentry->d_name, "r");
						
						strcpy(song->fileName, dentry->d_name);
						strcpy(song->songArtist, "NULLARTIST");
						strcpy(song->songTitle, "NULLTITLE");
						
						
						song_size = dstat->st_size;
						song_bitrate = calculateBitRate(songfile);	
						
						if (songfile != NULL) {
							
							song->songLength = calcSongLength(song_size, song_bitrate);
							fclose(songfile);
						}
						free(dstat);
					}
					printf("\n %-30s %.15s %.15s %10d", 
					song->fileName, 
					song->songArtist,
					song->songTitle,
					song->songLength);
					
					free(song);
				}
				
			}
			/*
			else if (dir->d_type == DT_DIR) {
				printf("[%s]\n", dir->d_name);
				if ( (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0) ) {
					parsedir(dir->d_name);
				}
			}
			*/
		}
	}
	closedir(dir);
}

int calcSongLength(int size_bytes, int bitrate) {
	/* calc filesize in bytes / 1024 to get filesize in Kbytes */
	/* multiply Kbytes * 8 to get Kbits */
	/* Divide Kbits by bitrate to get length in s */
	int length = (((size_bytes/1024) * 8) / bitrate);
	return length;
}

int calculateBitRate(FILE *fp) {
	/* TODO: Read file, get bits, calc actual bitrate */
	return 192;
}
