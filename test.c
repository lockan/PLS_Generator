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
int getID3TagSize(FILE * fp);
void getTitleName(FILE *fp, char *tnBuffer);	/* get song title from id3 */
void getArtistName(FILE *fp, char *anBuffer);	/* get artist from id3 */
int calcSongLength(int size_bytes, int bitrate); /* calc using filesize, bitrate */
int getFileSize(FILE *fp);						/* get from struct stat */
int calculateBitRate(FILE *fp);					/* calc data from bits id3 header block */
void debugSongStruct(struct songEntry *se);


int main(int argc, char *argv[])
{	
	plsFromDir(".");
	
	return(0);
}

void plsFromDir(char *dir_name) {
	int numEntries;		/* Number of songs in single playlist.  */
	DIR *dir;
	struct dirent *dentry;
	struct stat *dstat;
	struct songEntry *song;
	FILE *songfile;
	int song_size;
	int	song_bitrate;
	
	printf("Generating PLS from files in directory \" %s \" \n", dir_name);
	
	dir = opendir(dir_name);
	if (dir) {
		while ((dentry = readdir(dir)) != NULL) {
			if (dentry->d_type == DT_REG) {
				if ((song = malloc(sizeof(struct songEntry))) != NULL) {
					if ((dstat = malloc(sizeof(struct stat))) != NULL) {
						stat(dentry->d_name, dstat);
						songfile = fopen(dentry->d_name, "rb");
						
						strcpy(song->fileName, dentry->d_name);
						strcpy(song->songArtist, "NULLARTIST");
						strcpy(song->songTitle, "NULLTITLE");
						
						song_size = dstat->st_size;
						song_bitrate = calculateBitRate(songfile);
						
						if (songfile != NULL) {
							if (strstr(dentry->d_name, ".mp3") != NULL ) {
								getID3TagSize(songfile);
								song->songLength = calcSongLength(song_size, song_bitrate);
								fclose(songfile);
								
								debugSongStruct(song);
							}
						}
						free(dstat);
					}
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

int getID3TagSize(FILE *fp) {
	char *id3check;
	char *id3tagsize;
	int tagsize;
	
	if( (id3check = malloc(3)) != NULL) {
		if (fread(id3check, 3, 1, fp) != 0) {
			if (strncmp(id3check, "ID3", 3) == 0) {
				printf("\n[ valid mp3 found ]");
				printf("\ntag: %s", id3check);
				if( (id3tagsize = malloc(4)) != NULL ) {
					if (fseek(fp, 6, SEEK_SET) == 0) {
						if (fread(id3tagsize, 4, 1, fp) != 0) {
							printf("\ntagsize: %.3x %.3x %.3x %.3x", 
								id3tagsize[0],
								id3tagsize[1],
								id3tagsize[2],
								id3tagsize[3]
								);
						}
					}			
				}
			}
			else {
				printf("\n[ invalid mp3 ]");
			}
		}
	}
	
	if (id3tagsize != NULL) free(id3tagsize);
	if (id3check != NULL) free(id3check);
	
	return 0;
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
	return 268;
}

void debugSongStruct(struct songEntry *se) {
	printf("\n%s \n%s \n%s \n%d", 
		se->fileName, 
		se->songArtist,
		se->songTitle,
		se->songLength);
}
