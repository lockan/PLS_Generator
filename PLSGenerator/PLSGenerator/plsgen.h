#ifndef PLSGEN_H
#define PLSGEN_H

#include <string>
#include <fstream>
#include <D:/Code/libs/c++/boost_1_57_0/boost/filesystem.hpp>

using std::string;
using std::ofstream;
using boost::filesystem::path;

void playlistFromDir( boost::filesystem::path dir_name, int plsNum );
void writeTrackEntry( string songfile, ofstream *plsfile, const int entryNum );

#endif