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

# Script Setup
SAVEIFS=$IFS
IFS=$(echo -en "\n\b")

# database file
dbf=ConsoleMP.db 
# tmp file for storing tmp song data
mytmp=/tmp/songdata.junk
# tmp file for storing sql statements
sqlTMPFile=/tmp/sqlTMP.junk

# ***********************************************
# Functions
# ***********************************************
# \param $1 - The field to print contents of
function printField {
    echo $(cat $mytmp | grep $1 | head -1 | cut -d: -f2 | sed "s/'/''/g")

}

# \param $1 string to escape
# \return the escaped string
function escapeStrForSQLite {
    echo $1 | sed "s/'/''/g"
}

# ***********************************************
# Begin Main Script
# ***********************************************

# Create Database Table
rm $dbf
sqlite3 $dbf \
    'CREATE TABLE if not exists `songs` (
        `id`        INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
        `title`     TEXT NOT NULL,
        `artist`    TEXT NOT NULL,
        `album`     TEXT,
        `path`      TEXT NOT NULL UNIQUE,
        `year`      INTEGER,
        `rating`    INTEGER DEFAULT 0,
        `played`    INTEGER DEFAULT 0
     );'

# status msg
echo "Made SQL tables"

echo "PRAGMA synchronous=0;" > $sqlTMPFile
# Iterate over files in home folder to generate list of songs in SQL
# querry format, then dump it to file to be executed later.
echo "Searching for song data"
for f in $(find ~/Music -name "*.mp3") ; do
    path=$(escapeStrForSQLite $f ) # File Path
    ffprobe $path 2>&1 | tail -20 > $mytmp
    title=$(printField title)
    artist=$(printField artist)
    year=$(printField date)
    album=$(printField album)
    sql="Insert into songs(title, artist, year, album, path) values(
    '$title', '$artist', '$year', '$album', '$path');"
    echo $sql >> $sqlTMPFile
done

# Status Msgs
echo "Finished Collecting Song Data"
echo "Now Building Database"

sqlite3 $dbf < $sqlTMPFile
