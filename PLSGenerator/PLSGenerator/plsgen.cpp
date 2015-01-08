#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dirent.h"
#include <sys/stat.h>
#include "plsgen.h"

#include <D:/Code/libs/c++/boost_1_57_0/boost/filesystem.hpp>

#define TAGLIB_STATIC 

#include <D:/Code/libs/C++/taglib/include/taglib/fileref.h>
#include <D:/Code/libs/C++/taglib/include/taglib/tag.h>

using namespace std;
using namespace TagLib;
using namespace boost::filesystem;


int main(void) {

    using boost::filesystem::path;
    
    cout << "<<<< RUNNING PLSGEN >>>>>" << endl;

    boost::filesystem::path boost_runpath = current_path( );
    //stringstream runpath; 
    //runpath << path( boost_curpath ).string( );
    cout << "RunPath: " << boost_runpath << endl;

    int playListCount = 0;
    //playlistFromDir( runpath.str(), ++playListCount );
    playlistFromDir( boost_runpath, playListCount );

    //TODO: Added some sort of stats counter to return the number of playlists successfully generated. 

    cout << endl << "<<<<< PLSGEN FINISHED >>>>>" << endl << endl;
    cin.get( );
    
    return 0;
}
void playlistFromDir(path dir_name, int plsNum )
//void playlistFromDir( string dir_name, int plsNum )
{
    cout << "Scanning directory " << dir_name.string() << endl; //dir_name is same as runpath in main. (Full file path). 
    
    stringstream pls_name;
    pls_name << dir_name.string() << "\\" << dir_name.filename().string() << ".pls";
    //cout << "PLS NAME: " << pls_name.str() << endl;

    ofstream plsfile;   
    //TODO: Error check the open() function. 
    plsfile.open( pls_name.str(), std::ios_base::trunc );
    if ( !plsfile.is_open( ) )
    {
        cout << "ERROR: FAILED TO CREATE PLAYLIST" << endl;
        cout << "Returning, Skipping directory" << endl;
        return;
    }
    plsfile << "[playlist]" << endl << endl;

    DIR *dir;
    dir = opendir( dir_name.string().c_str() );
    //dir = opendir( dir_name.c_str() );
    //error-check opendir here. 
    
    struct stat *dstat;
    dstat = ( struct stat* )malloc( sizeof( struct stat ) );
        
    struct dirent *dentry;

    int numEntries = 0;
            
    while ( ( dentry = readdir( dir ) ) != NULL )
    {
        // HANDLE FILES
        if ( dentry->d_type == DT_REG )
        {
            stat( dentry->d_name, dstat );
            //error-check stat here.
                
            // HANDLE FILES WITH MP3 EXTENSION 
            if ( strstr( dentry->d_name, ".mp3" ) != NULL )
            {
                cout << "[SONG]: " << dentry->d_name << endl;
                
                ++numEntries; 
                //plsfile << "File" << numEntries << "=" << dir_name.string() << "\\" << dentry->d_name << endl;
                plsfile << "File" << numEntries << "=" << dentry->d_name << endl; // Only using relative path rather than absolute. Just the filenamne in this case.
                //writeTrackEntry( dentry->d_name, &plsfile, numEntries );
                stringstream songPathString;
                songPathString << dir_name.string( ) << "\\" << dentry->d_name;
                path songPath = path( songPathString.str( ) );
                writeTrackEntry( songPath, &plsfile, numEntries ); //TODO: requires absolute file path to find file. 
            }
            // HANDLE(IGNORE) NON-MP3, NON-DIR FILES
            else
            {
                //cout << "[file]: " << dentry->d_name << endl;
            }
        }
        // HANDLE DIRECTORIES
        else if ( dentry->d_type == DT_DIR )
        {
            //cout << "[dir ]: " << dentry->d_name << endl;
            if ( string( dentry->d_name ).compare( "." ) != 0 && string( dentry->d_name ).compare( ".." ) != 0 )
            {
                stringstream newPathString;
                newPathString << dir_name.string() << "\\" << dentry->d_name;
                path newPath = path( newPathString.str() );
                playlistFromDir( newPath, ++plsNum );
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
        cout << endl << "CREATED PLAYLIST: " << pls_name.str( ) << endl << endl;
    }
    
    if ( numEntries == 0 )
    {
        remove( pls_name.str().c_str() );
    }
    
    return;
}
void writeTrackEntry( path songfile, ofstream *plsfile, const int entryNum )
//void writeTrackEntry( string songfile, ofstream *plsfile, const int entryNum )
{
    stringstream songpath; 
    songpath << songfile.string();

    //cout << "DEBUG: SONGPATH: " << songpath.str() << endl;
    //cout << "DEBUG: SONFILE: " << songfile << endl;

    ifstream sfile;
    sfile.open( songpath.str( ), ios_base::binary );
    if ( !sfile.is_open() ) cout << "ERROR: Failed to open file " << songfile << endl;
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
