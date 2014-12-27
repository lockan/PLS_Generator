#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
//#include <C:/cygwin64/usr/include/dirent.h>
#include "dirent.h"
#include <sys/stat.h>
#include "plsgen.h"

#define TAGLIB_STATIC 

#include <D:/Code/libs/C++/taglib/include/taglib/fileref.h>
#include <D:/Code/libs/C++/taglib/include/taglib/tag.h>

using namespace std;
using namespace TagLib;


int main(void) {

    //plsFromDir( "D:\\Code\\C++\\PLS_Generator\\PLSGenerator\\PLSGenerator" );
    walkDir( "D:\\Code\\C++\\PLS_Generator\\PLSGenerator\\PLSGenerator" );

    cin.get( );
    
    return 0;
}

void walkDir( char *dir_name )
{
    //TODO: find a better way to name the playlist file. 
    //TODO: If need be, name it "temp.pls" and rename it to Album Title later. 
    //Alternate: split the path string into an array using \ as delim, take the last item as the name 

    string pls_name = dir_name;
    pls_name.append( "\\playlist.pls" );
     
    ofstream plsfile;   
    plsfile.open( pls_name, std::ios_base::trunc );
    plsfile << "[playlist]" << endl << endl;

    printf( "Generating PLS from files in directory \" %s \" \n", dir_name );

    DIR *dir;
    dir = opendir( dir_name );
    cout << dir_name << endl;
    //error-check opendir here. 
    
    struct stat *dstat;
    dstat = ( struct stat* )malloc( sizeof( struct stat ) );
        
    struct dirent *dentry;
            
    while ( ( dentry = readdir( dir ) ) != NULL )
    {
        if ( dentry->d_type == DT_REG )
        {
            stat( dentry->d_name, dstat );
            //error-check stat here.
                
            if ( strstr( dentry->d_name, ".mp3" ) != NULL )
            {
                cout << "[SONG]: " << dentry->d_name << endl;
                writeTrackEntry( dentry->d_name, &plsfile );
            }
            else
            {
                cout << "[file]: " << dentry->d_name << endl;
            }
        }
        else if ( dentry->d_type == DT_DIR )
        {
            cout << "[dir ]: " << dentry->d_name << endl;
        }
    }
    plsfile.close( );
    closedir( dir );
    std::free( dstat );  
}

void writeTrackEntry( string songfile, ofstream *plsfile )
{
    ifstream sfile;
    sfile.open( songfile, ios_base::binary );
    // error-check open() here. 
    
    TagLib::FileName fn( songfile.c_str() );
    TagLib::FileRef fr( fn, true, TagLib::AudioProperties::Accurate );
    
    if ( !fr.isNull( ) )
    {
        //TODO: prefix each line with FILE1, TITLE1, etc. 
        TagLib::Tag *tag = fr.tag( );
        *plsfile << songfile << endl;
        *plsfile << tag->artist( ) << " - " << tag->title( ) << endl;
        *plsfile << "Length: " << fr.audioProperties( )->length( ) << endl << endl;
    }

    sfile.close( );
    //error-check close() here.
}