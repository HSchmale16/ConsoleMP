/**\file   sql.h
 * \brief  Constain various declarations for handling the database of
 *         the console media player.
 * \author Henry J Schmale
 * \date   April 4, 2015
 */

#ifndef SQL_H_INC
#define SQL_H_INC

const char SQL_SONGS_TB_CREATE = 
"CREATE TABLE `songs` ("
    "`id`    INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
    "`title` TEXT NOT NULL,"        //!< Song Title
    "`artist`TEXT NOT NULL,"        //!< Who wrote
    "`album` TEXT,"                 //!< Album
    "`path`  TEXT NOT NULL UNIQUE," //!< File path
    "`year`  INTEGER,"              //!< Date Created
    "`rating`INTEGER DEFAULT 0,"    //!< 0-5 Star Rating
    "`played`INTEGER DEFAULT 0,"    //!< Times Played
    "`tags`  TEXT"                  //!< tags for this song
    ");"

#endif // SQL_H_INC
