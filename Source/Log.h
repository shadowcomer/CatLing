/*
    This file contains a Log. It uses the Singleton pattern.

    This Log implementation gives the possibility to choose the minimum level of
    messages that will be logged.

    A single file may be written to at any time. Files are truncated when opened.

    When using this class, use the given LOG(x) and LOG(x,y) macros for shorter
    and more legible calls.

    Thread safe.
*/

#include <fstream>
#include <stdlib.h>
#include <string>
#include <time.h>

#include "../include/TBB/tbb/mutex.h"

enum class LOG_LEVEL
{ DEBUG3, DEBUG2, DEBUG1, INFO, WARNING, PR_ERROR, BUG, CRITICAL };

/*
Converts the given LOG_LEVEL into it's corresponding string.
*/
std::string textifyLogLevel(LOG_LEVEL);

#define LOG(x) Log::Instance()->writeToFile( x ) ;
#define LOGL(x,y) Log::Instance()->writeToFile( x , y ) ;

class Log{
public:
    /*
    Singleton pattern instance retrieval.

    If the Log hasn't been constructed yet, it constructs it and returns it as a result.
    If it has been constructed, it returns it.
    */
    static Log* Instance();

    /*
    Opens a new log file. The previous file must be closed before opening a new one.
    Truncates on opening.
    */
    int openLogFile(char* fileName);

    /*
    Closes the current log file.
    */
    void closeLogFile();

    /*
    Writes to the currently open log file with default LOG_LEVEL::INFO.
    */
    void writeToFile(char* text);

    /*
    Writes to the currently open log file. Only messages that are of
    m_capLevel or higher will be logged.
    */
    void writeToFile(LOG_LEVEL enumEntry, char* text);

    /*
    Changes the current m_capLevel.
    */
    void changeLevel(LOG_LEVEL newLevel);

private:
    Log();
    Log(Log const&)=delete;
    Log& operator=(Log const&)=delete;

    tbb::mutex SYNC_operation;

    std::ofstream m_logFile;

    LOG_LEVEL m_capLevel;
};