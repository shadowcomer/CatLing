#include <stdio.h>

#include <assert.h>

#include "Log.h"

std::string textifyLogLevel(LOG_LEVEL level)
{
    // TODO: Check if this can be converted into a reference, so it
    // doesn't have to be copied each time we decode it.
    switch(level)
    {
    case LOG_LEVEL::DEBUG1:     return "DEBUG1";
    case LOG_LEVEL::DEBUG2:     return "DEBUG2";
    case LOG_LEVEL::DEBUG3:     return "DEBUG3";
    case LOG_LEVEL::INFO:       return "INFO";
    case LOG_LEVEL::WARNING:    return "WARNING";
    case LOG_LEVEL::PR_ERROR:   return "ERROR";
    case LOG_LEVEL::BUG:        return "BUG";
    case LOG_LEVEL::CRITICAL:   return "CRITICAL";
    default:                    return "-NDEF-";
    }
}

Log::Log() :
m_capLevel(LOG_LEVEL::INFO)
{

}

Log* Log::Instance()
{
    // TODO: Check if it's safe to return a pointer to a
    // static variable within a function.
    static Log logInstance;
    return &logInstance;
}

void Log::changeLevel(LOG_LEVEL newLevel){
    // NOTE: If a write has already started but the level, at the moment
    // before the check, wouldn't go through, then a change to the caplevel
    // before the check would allow it to be written, even if it's posterior
    // to the beginning of the write call.
    m_capLevel = newLevel;
}

int Log::openLogFile(char* fileName){
    {
        tbb::mutex::scoped_lock lock(SYNC_operation);
        if (m_logFile.is_open()){
            return -1;
        }

        m_logFile.open(fileName, std::ofstream::out | std::ofstream::trunc);
        return m_logFile.is_open() ? 0 : -2;
    }
}

void Log::writeToFile(char* text)
{
    writeToFile(LOG_LEVEL::INFO, text);
}

void Log::writeToFile(LOG_LEVEL level, char* text)
{
    if (!m_logFile.is_open()){
        return;
    }

    if (level < m_capLevel){
        return;
    }

    struct tm timeInfo;
    __time64_t long_time;
    char timeStr[64];
    std::string levelText(textifyLogLevel(level));

    errno_t err;

    _time64(&long_time);
    err = localtime_s(&timeInfo, &long_time);
    assert(err == 0);

    err = sprintf_s(timeStr, "%d:%d:%d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    assert(err == 0);

    {
        tbb::mutex::scoped_lock lock(SYNC_operation);
        // We have to check again because the rest of the
        // function isn't within the lock.
        if (!m_logFile.is_open()){
            return;
        }

        m_logFile << timeStr << " " << levelText << ": "
            << text << std::endl;
        m_logFile.flush();
    }
}

void Log::closeLogFile(){
    {
        tbb::mutex::scoped_lock lock(SYNC_operation);
        if (m_logFile.is_open()){
            m_logFile.close();
        }
    }
}





