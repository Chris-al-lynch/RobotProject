
#include "utils.h"

template <typename T>
string intToHexString( T num )
{
    stringstream stream;
    stream << "0x" << setfill( '0' ) << setw( sizeof( T ) * 2 ) << hex << num;

    return stream.str();
}