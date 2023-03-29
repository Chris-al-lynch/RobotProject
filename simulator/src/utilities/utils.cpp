#include <iomanip>
#include <sstream>

#include "utils.h"

string intToHexString( int num )
{
    stringstream stream;
    stream << "0x" << setfill( '0' ) << setw( sizeof( int ) * 2 ) << hex << num;

    return stream.str();
}