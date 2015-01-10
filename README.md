# PLS_Generator
Created by A.Lockhart 2015. 

[Description]
PlsGen is a playlist generator for MP3 files. It is fast solution for generating a large number of 
playlist files with very little effort. It is designed to work on the Windows platform. 
Currently there is no support for other platforms. 

[Technical Details]
PlsGen is written in C++ and uses the standard C++ libraries. 
It also uses the TagLib project for reading ID3 tags from the MP3 files. 
The C++ Boost libraries have been used to simplify working with file paths. 
A C++ implementation of dirent.h is used to crawl the directories. This was implemented
before the addition of Boost, but in future the code that uses dirent.h may be replaced by 
classes and features of the Boost libraries. 

[.PLS Format]
Playlist files are created in the .pls format. 
It is a simple text-based format that should be supported by most PC and mobile devices.

[How PlsGen Works]
PlsGen considers each folder a playlist. 
It will scan a directory tree recursively starting from the folder in which it is executed.
If any MP3 files are encountered it will create a playlist (.pls) file for those MP3 files. 
The file is created in the same directory as the found MP3s, and is named using the folder name. 

Example: 
If you run PlsGen.exe from the root of your C:\iTunes\Media\Music\ folder and you 
have 100 albums each in their own folder, PlsGen will generate 100 playlists - one for each folder/album
and in their respective folders. 

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

