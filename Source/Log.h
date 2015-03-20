#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum logLvlEnum { DEBUG3, DEBUG2, DEBUG1, INFO, WARNING, ERROR, BUG, CRITICAL };

#define LOG(x) Log::Instance()->writeToFile( x ) ;
#define LOGLVL(x,y) Log::Instance()->writeToFile( x , y ) ;

class Log{
public:
	static Log* Instance();
	static FILE* m_fichero;
	int openLogFile(char* logfile);
	void writeToFile(char* entrada);
	void writeToFile(logLvlEnum enumEntry, char* entrada);
	int closeLogFile();
	void logLevel(int num);

private:
	Log(){};
	Log(Log const&){};
	Log& operator=(Log const&){};
	static Log* m_pInstance;
	static char* numtostring[8];
	int caplevel;



};