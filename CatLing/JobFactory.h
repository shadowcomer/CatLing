#ifndef JOBFACTORY_H
#define JOBFACTORY_H

#include "Job.h"

/*
	JobFactory is in charge of the generation of Jobs. An interface in MThread
	will serve as an entrypoint for job generation.
*/

#include <boost\heap\fibonacci_heap.hpp>

class JobFactory
{
public:
	JobFactory();
	~JobFactory();

	// Entry point for job requests
	bool requestJob(/*TODO*/);
	bool cancelJob(/*TODO*/);
	bool queryJob(/*TODO*/);

protected:

private:
	boost::heap::fibonacci_heap<Job> m_jobs;

};

#endif