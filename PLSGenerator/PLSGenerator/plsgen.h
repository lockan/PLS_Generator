#ifndef PLSGEN_H
#define PLSGEN_H

#include <string>
#include <fstream>

using std::string;
using std::ofstream;

void playlistFromDir( string dir_name, int plsNum );
void writeTrackEntry( string songfile, ofstream *plsfile, const int entryNum );

#endif