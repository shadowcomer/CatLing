#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum zas {DEBUG0,DEBUG1,DEBUG2,DEBUG3};
#define LOG(x) ErrorLogTone::Instance()->writeToFile( x ) ;
#define LOGLVL(x,y) ErrorLogTone::Instance()->writeToFile( x , y ) ;

class ErrorLogTone{
public:
	static ErrorLogTone* Instance();
	static FILE* m_fichero;
	int openLogFile(char* logfile);
	void writeToFile(char* entrada);
	void writeToFile(zas hack,char* entrada) ;
	int closeLogFile();

private:
	ErrorLogTone(){};
	ErrorLogTone(ErrorLogTone const&){};
	ErrorLogTone& operator=(ErrorLogTone const&){};
	static ErrorLogTone* m_pInstance;


};