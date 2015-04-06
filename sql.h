/**\file   sql.h
 * \brief  Constain various declarations for handling the database of
 *         the console media player.
 * \author Henry J Schmale
 * \date   April 4, 2015
 */

#ifndef SQL_H_INC
#define SQL_H_INC

#include <string>
#include <sqlite3.h>

/**\brief The song table creation querry
 */
const char SQL_SONGS_TB_CREATE[] = 
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
    ");";

/**\brief The database file path
 */
const char DB_FILE[] = "ConsleMP.sqlite";

/**\brief Struct to hold each song record
 */
struct SongData{
    std::string m_title;  //!< Title of the song
    std::string m_artist; //!< Group who did the song
    std::string m_path;   //!< Path to that file
};

/**\brief Media Database Class
 *
 * A class with the abilty to build sql querries and return the data
 * returned by those querries. Has the ability to be highly customized
 * in instation.
 */
class MediaDB{
public:
    MediaDB();
    ~MediaDB();
private:
    sqlite3 *m_db;

};

#endif // SQL_H_INC
