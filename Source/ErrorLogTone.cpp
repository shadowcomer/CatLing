#include <stdio.h>
#include "errorlogtone.h"

ErrorLogTone* ErrorLogTone::m_pInstance = NULL;
FILE* ErrorLogTone::m_fichero = NULL;


/*
*****************************************************
Devuelve siempre la misma instancia
de ErrorLogTone
*****************************************************
*/
ErrorLogTone* ErrorLogTone::Instance()
{
	if (!m_pInstance){
		m_pInstance= new ErrorLogTone;
	}
	return m_pInstance;
}


/*
*****************************************************
Sustituye o crea el fichero con el nombre indicado
por el parámetro.
*****************************************************
*/
int ErrorLogTone::openLogFile(char* file){
	
	if(m_fichero==NULL){
		m_fichero = fopen(file,"w+");
			if(m_fichero==NULL)
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
Escribe en el fichero de log el
string que se pasa por parámetro
*****************************************************
*/
void ErrorLogTone::writeToFile(char* entrada) 
{
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(buffer,80,"[%I:%M:%S] ",timeinfo);

	strcat(buffer,entrada);

		fwrite(buffer,1,strlen(buffer),m_fichero);
	
}
/*
*****************************************************
Escribe en el fichero de log el
string que se pasa por parámetro y ademas
el nivel de debug
*****************************************************
*/
void ErrorLogTone::writeToFile(zas hack,char* entrada) 
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(buffer,80,"[%I:%M:%S] ",timeinfo);

	char mega[128];
		itoa(hack,mega,10);
	char * loginfo=strcat(mega," ");
	  loginfo=strcat(buffer,mega);
	  loginfo=strcat(buffer,entrada);
		fwrite(buffer,1,strlen(buffer),m_fichero);
	
}

/*
*****************************************************
Cierra el fichero que está abierto
*****************************************************
*/
int ErrorLogTone::closeLogFile(){
	fclose(m_fichero);
	return 0;
}



