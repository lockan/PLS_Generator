#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dirent.h"
#include <sys/stat.h>
#include "plsgen.h"

#include <D:/Code/libs/C++/boost_1_57_0/boost/algorithm/string/replace.hpp>
#include <D:/Code/libs/c++/boost_1_57_0/boost/filesystem.hpp>

#define TAGLIB_STATIC 

#include <D:/Code/libs/C++/taglib/include/taglib/fileref.h>
#include <D:/Code/libs/C++/taglib/include/taglib/tag.h>

using namespace std;
using namespace TagLib;
using namespace boost::filesystem;


int main(void) {

    using boost::replace_all;
    
    cout << "<<<< RUNNING PLSGEN >>>>>" << endl;

    boost::filesystem::path boost_curpath = current_path( );
    stringstream runpath; 
    runpath << path( boost_curpath ).string( );
    
    cout << "Current Path: " << runpath.str() << endl;

    int playListCount = 0;
    playlistFromDir( runpath.str(), ++playListCount );

    cout << "<<<<< PLSGEN FINISHED >>>>>" << endl;
    cin.get( );
    
    return 0;
}

void playlistFromDir( string dir_name, int plsNum )
{
    cout << "Scanning directory " << dir_name << endl;
    
    //TODO: find a better way to name the playlist file. 
    //If need be, name it "temp.pls" and rename it to Album Title later. 
    //Alternate: split the path string into an array using \ as delim, take the last item as the name 
    stringstream pls_name;
    pls_name << dir_name << "\\playlist" << plsNum << ".pls";
    
    ofstream plsfile;   
    plsfile.open( pls_name.str(), std::ios_base::trunc );
    if ( !plsfile.is_open( ) ) cout << "FAILED TO CREATE PLAYLIST" << endl;
    
    plsfile << "[playlist]" << endl << endl;

    DIR *dir;
    dir = opendir( dir_name.c_str() );
    //error-check opendir here. 
    
    struct stat *dstat;
    dstat = ( struct stat* )malloc( sizeof( struct stat ) );
        
    struct dirent *dentry;

    int numEntries = 0;
            
    while ( ( dentry = readdir( dir ) ) != NULL )
    {
        if ( dentry->d_type == DT_REG )
        {
            stat( dentry->d_name, dstat );
            //error-check stat here.
                
            if ( strstr( dentry->d_name, ".mp3" ) != NULL )
            {
                cout << "[SONG]: " << dentry->d_name << endl;
                
                ++numEntries;
                plsfile << "File" << numEntries << "=" << dir_name << "\\" << dentry->d_name << endl;
                writeTrackEntry( dentry->d_name, &plsfile, numEntries );
            }
            else
            {
                //cout << "[file]: " << dentry->d_name << endl;
            }
        }
        else if ( dentry->d_type == DT_DIR )
        {
            //cout << "[dir ]: " << dentry->d_name << endl;
            if ( string( dentry->d_name ).compare( "." ) != 0 && string( dentry->d_name ).compare( ".." ) != 0 )
            {
                stringstream newPath;
                newPath << dir_name << "\\" << dentry->d_name;
                playlistFromDir( newPath.str().c_str(), ++plsNum );
            }
        }
    }
    
    plsfile << "NumberOfEntries=" << numEntries << endl << endl;
    plsfile << "Version=2" << endl;

    plsfile.close( );
    closedir( dir );
    std::free( dstat );

    if ( numEntries > 0 )
    {
        cout << "Created Playlist: " << pls_name.str( ) << endl;
    }
    
    if ( numEntries == 0 )
    {
        remove( pls_name.str().c_str() );
    }
    
    return;
}

void writeTrackEntry( string songfile, ofstream *plsfile, const int entryNum )
{
    ifstream sfile;
    sfile.open( songfile, ios_base::binary );
    if ( !sfile.is_open() ) cout << "Failed to open file " << songfile << endl;
    // error-check open() here. 
    
    TagLib::FileName fn( songfile.c_str() );
    TagLib::FileRef fr( fn, true, TagLib::AudioProperties::Accurate );
    
    if ( !fr.isNull( ) )
    { 
        TagLib::Tag *tag = fr.tag( );
        *plsfile << "Title" << entryNum << "=" << tag->artist( ) << " - " << tag->title( ) << endl;
        *plsfile << "Length" << entryNum << "=" << fr.audioProperties( )->length( ) << endl << endl;
    }

    sfile.close( );
    //error-check close() here.
}
