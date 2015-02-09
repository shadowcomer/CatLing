#ifndef JOB_H
#define JOB_H


/*
	A job is an encapsulation unit for a function call request. Every call has to
	go through the MThread because of the BWAPI's inability to work in with concurrent
	threads.

	The functionality of a job is not the functionality of BWAPI. Jobs have to be completely
	safe. A job just encapsulates a function with calling safety measures.

	For example: GetUnits
*/
class CatlingModule;

class Job
{
public:
	Job(/*TODO: Jobs can't be created by default*/);
	~Job();

protected:
	bool execute(/*TODO*/);

private:
	CatlingModule* m_caller;

	int m_id;
	unsigned int m_nParams;
	char ** m_params;

	bool validateParameters(/*TODO*/);

};

#endif