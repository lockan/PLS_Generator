# PLS_Generator
Created by A.Lockhart 2015. 

**[Description]**
<br>PlsGenerator is a playlist generator for MP3 files. It is fast solution for generating a large number of 
playlist files with very little effort. It is designed to work on the Windows platform. 
Currently there is no support for other platforms. 

**[Technical Details]**
<br>PlsGen is written in C++ and uses the standard C++ libraries. 
It also uses the TagLib project for reading ID3 tags from the MP3 files. 
The C++ Boost libraries have been used to simplify working with file paths. 
A C++ implementation of dirent.h is used to crawl the directories. This was implemented
before the addition of Boost, but in future the code that uses dirent.h may be replaced by 
classes and features of the Boost libraries. 

**[.PLS Format]**
<br>Playlist files are created in the .pls format. 
It is a simple text-based format that should be supported by most PC and mobile devices.

**[How PlsGen Works]**
<br>PlsGen considers the contents of each folder a potential playlist. 
By default (no optional arguments) it will scan the directory in which it is executed. If any MP3 files are encountered it will create a playlist (.pls) file for those MP3 files. The .pls file is created in the same directory as the found MP3s using the folder name. Folders that do not contain MP3 files are ignored. 

**[FileNames]**
<br>Playlists are named by the root folder that contains the MP3 files. 
e.g. if C:\Music\Mp3s\CoolGuy\ contains Mp3s, the file "CoolGuy.pls" will be created.

*Example 1: Single Folder* 
<br>If you run PlsGenerator.exe from the E:\Music\MyCoolBand\ folder (with no additional arguments) it will generate the file E:\Music\MyCoolBand\MyCoolBand.pls, assuming it encounters MP3s in that folder. 

*Example 2: Recursive Mode* 
<br>In recursive mode (using the "-r" argument) if you run PlsGenerator.exe from the root of your C:\iTunes\Media\Music\ folder and you have 100 albums each in their own folder, PlsGenerator will generate 100 playlists - one for each folder/album
and in their respective folders. 

**[*NEW* Optional Command-Line Arguments]**
- -h: [H]elp. Will display a help/usage message.
- -t {path}: [T]argeted
- -r: [R]ecursive mode. Will scan all subdirectories recursively starting from the current directory, or the target directory if used in conjunction with -t. 
- -s: [S]imple mode. Does not include the [title] line in the playlist file. This should be used for compatability in cases where the player software shows duplicate track entries for the same file. 

*Warning:* playlist file creation is destructive in nature. If a .pls with the same name already exists in 
the location PlsGen is targeting, that playlist will be overwritten automatically and without warning. 

**[Track Ordering]**
<br>Tracks/files will be ordered in the order in which they are encountered in the folder. 
E.g. 
- If prefixed by track number, the tracks will be ordered by track number. 
- If the files are alphabetical the tracks will be alphabetical in order. 

All file references are written as relative paths to ensure that the playlists are portable
between your PC and other portable devices. 

