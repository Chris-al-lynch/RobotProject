#!/usr/bin/python

from os import walk, remove
from os.path import join
from shutil import rmtree

def main():
    cmakeFileList = ["CMakeCache.txt", "Makefile", "cmake_install.cmake"]
    cmakeDirList  = ["CMakeFiles", "bin"]

    for currentPath, dirs, files in walk( "." ):
        for file in files:
            if( file in cmakeFileList ):
                remove( join( currentPath, file ) )

        for dir in dirs:
            if( dir in cmakeDirList ):
                rmtree( join( currentPath, dir ) )

if( __name__ == "__main__" ):
    main()

