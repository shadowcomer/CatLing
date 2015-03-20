#include <stdio.h>
#include "Log.h"

Log* Log::m_pInstance = NULL;
FILE* Log::m_fichero = NULL;
char* Log::numtostring[8] = { "DEBUG3", "DEBUG2", "DEBUG1", "INFO", "WARNING", "ERROR", "BUG", "CRITICAL" };

/*
*****************************************************
Allways returns the same instance for the log.
*****************************************************
*/
Log* Log::Instance()
{
	if (!m_pInstance){
		m_pInstance = new Log;
	}
	return m_pInstance;
}

/*
*****************************************************
Log level priority. It assigns the priority neccesary
for the logs to be printed in the log file.
*****************************************************
*/
void Log::logLevel(int num){
	Log::caplevel = num;
}


/*
*****************************************************
Creates or replaces the old file with a new one.
*****************************************************
*/
int Log::openLogFile(char* file){

	if (m_fichero == NULL){
		m_fichero = fopen(file, "w+");
		if (m_fichero == NULL)
		{
			return -2;
		}

		return 0;
	}
	else
	{
		return -1;
	}


}
/*
*****************************************************
Prints on the log file the string.
*****************************************************
*/
void Log::writeToFile(char* entrada)
{

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "[%I:%M:%S] ", timeinfo);

	strcat(buffer, entrada);

	fwrite(buffer, 1, strlen(buffer), m_fichero);

}
/*
*****************************************************
Checks the loglevel and prints on the log file the
string if it's priority is higher
*****************************************************
*/
void Log::writeToFile(logLvlEnum enumEntry, char* entrada)
{
	if (enumEntry >= Log::caplevel){
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "[%I:%M:%S] ", timeinfo);

		char* loginfo = strcat(buffer, Log::numtostring[enumEntry]);
		loginfo = strcat(buffer, ":");
		loginfo = strcat(buffer, entrada);
		fwrite(buffer, 1, strlen(buffer), m_fichero);
	}
}

/*
*****************************************************
Closes the current file's descriptor
*****************************************************
*/
int Log::closeLogFile(){
	fclose(m_fichero);
	return 0;
}





