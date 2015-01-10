# PLS_Generator
Created by A.Lockhart 2015. 

[Description]
PlsGen is a playlist generator for MP3 files. It is fast solution for generating a large number of 
playlist files with very little effort. It is designed to work on the windows platform. 
Currently there is no support for other platforms. 

[Technical Details]
PlsGen is written in C++ and uses the standard C++ libraries. 
It also uses the TagLib project for reading ID3 tags from the MP3 files. 
The C++ Boost libraries have been used to simplify working with file paths. 
A C++ implementation of dirent.h is used to crawl the directories. This was done
before the addition of boost, but in future dirent.h may be replaced completely by using only
boost libraries. 

[.PLS Format]
Playlist files are created i the .pls format. 
It is a simple text-based format that should be supported by most PC and mobile devices.

[How PlsGen Works]
PlsGen considers each folder a playlist. 
It will scan a directory tree recursively starting from the folder in which it is executed.
If any MP3 files are encountered it will create a playlist (.pls) file for those MP3 files. 

[FileNames]
Playlists are named by the root folder that contains the MP3 files. 
e.g. if C:\Music\Mp3s\CoolGuy\ contains Mp3s, the file "CoolGuy.pls" will be created. 

[Track Ordering]
Tracks/files will be ordered in the order in which they are encountered in the folder. 
E.g. 
- If prefixed by track number, the tracks will be ordered by track number. 
- If the files are alphabetical the tracks will be alphabetical in order. 

All file references are written as relative paths to ensure that the playlists are portable
between your PC and other portable devices. 

