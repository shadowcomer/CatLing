#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum zas {DEBUG0,DEBUG1,DEBUG2,DEBUG3};
#define LOG(x) Log::Instance()->writeToFile( x ) ;
#define LOGLVL(x,y) Log::Instance()->writeToFile( x , y ) ;

class Log{
public:
	static Log* Instance();
	static FILE* m_fichero;
	int openLogFile(char* logfile);
	void writeToFile(char* entrada);
	void writeToFile(zas hack,char* entrada) ;
	int closeLogFile();

private:
	Log(){};
	Log(Log const&){};
	Log& operator=(Log const&){};
	static Log* m_pInstance;


};