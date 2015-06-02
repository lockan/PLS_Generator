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

bool mode_Recursive = false;
bool mode_Simple = false;
bool mode_Targeted = false;
boost::filesystem::path startDir;

int main(int argc, char** argv) {

    using boost::filesystem::path;

    cout << "<<<< RUNNING PLSGEN >>>>>" << endl;

    checkOptions( &argc, argv );

    boost::filesystem::path boost_runpath = current_path( );
    if ( !mode_Targeted ) startDir = boost_runpath;
    cout << "RunPath: " << startDir << endl;

    int playListCount = 0;
    playlistFromDir( boost_runpath, playListCount );

    //TODO: Add some sort of stats counter to return the number of playlists successfully generated. 

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
                
                plsfile << "File" << numEntries << "=" << dentry->d_name << endl; // Only using relative path rather than absolute. Just the filenamne in this case.
                
                stringstream songPathString;
                songPathString << dir_name.string( ) << "\\" << dentry->d_name;
                path songPath = path( songPathString.str( ) );
                writeTrackEntry( songPath, &plsfile, numEntries ); //TODO: requires absolute file path to find file. 
            }
            // HANDLE(IGNORE) NON-MP3, NON-DIR FILES
            else
            {
                //Do nothing - ignore file
            }
        }
        // HANDLE DIRECTORIES
        else if ( dentry->d_type == DT_DIR && mode_Recursive)
        {
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

    ifstream sfile;
    sfile.open( songpath.str( ), ios_base::binary );
    if ( !sfile.is_open() ) cout << "ERROR: Failed to open file " << songfile << endl;
    // error-check open() here. 
    
    TagLib::FileName fn( songfile.c_str() );
    TagLib::FileRef fr( fn, true, TagLib::AudioProperties::Accurate );
    
    if ( !fr.isNull( ) )
    { 
        TagLib::Tag *tag = fr.tag( );
        if ( !mode_Simple )
        {
            *plsfile << "Title" << entryNum << "=" << tag->artist( ) << " - " << tag->title( ) << endl;
        }
        *plsfile << "Length" << entryNum << "=" << fr.audioProperties( )->length( ) << endl << endl;
    }

    sfile.close( );
    //error-check close() here.
}

void checkOptions(const int *argsnum, char **args )
{
    for ( int i = 1; i < *argsnum; i++ )
    {
        if ( string( args[ i ] ).compare( "-r" ) == 0 )
        {
            //Enable recursize mode
            mode_Recursive = true;
            cout << "Recursive mode enabled." << endl;
        }
        if ( string( args[ i ] ).compare( "-s" ) == 0 )
        {
            //Enable simple mode
            mode_Simple = true;
            cout << "Simple mode enabled" << endl;
        }
        if ( string( args[ i ] ).compare( "-t" ) == 0 )
        {
            //Set target directory
            if ( args[ i + 1 ] != NULL )
            {
                mode_Targeted = true;
                startDir = string( args[ i + 1 ] );
                cout << "Target start directory set: " << startDir << endl;
            }
            else
            {
                //TODO: abort, print usage. 
                cout << "Missing target dir. Running in current dir." << endl;
            }
        }
    }
}
