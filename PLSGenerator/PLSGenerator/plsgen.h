#ifndef PLSGEN_H
#define PLSGEN_H

#include <string>
#include <fstream>

using std::string;
using std::ofstream;

void walkDir( char *dir_name );
void writeTrackEntry( string songfile, ofstream *plsfile );

#endif