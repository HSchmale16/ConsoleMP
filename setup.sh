# Setup Script for ConsoleMP
# Builds the database, and finds songs
# Henry J Schmale
# March 18, 2015

# database file
dbf=ConsoleMP.db 

# Create Database Table
sqlite3 $dbf "Create Table if not exists 
                     SONGS(ID INTEGER PRIMARY KEY,
                           NAME TEXT,
                           AUTHOR TEXT,
                           PATH TEXT,
                           RATING INTEGER,
                           PLAYCOUNT INTEGER);"

