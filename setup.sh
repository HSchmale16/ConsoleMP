#!/bin/bash
# Setup Script for ConsoleMP
# Builds the database, and finds songs
# Henry J Schmale
# March 18, 2015
# 
# ***********************************************
# Depends:
# * ffprobe
# * sqlite3


SAVEIFS=$IFS
IFS=$(echo -en "\n\b")

# database file
dbf=ConsoleMP.db 
# tmp file for storing tmp data
mytmp=/tmp/songdata.junk

# Create Database Table


# Iterate over files in home folder to generate list of songs
for f in $(find ~/Music -name "*.mp3") ; do
    path=$f                            # File Path
    ffprobe $path 2>&1 | tail -20 > $mytmp
        
done
