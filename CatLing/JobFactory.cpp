#include "JobFactory.h"

JobFactory::JobFactory():
	m_jobs(boost::heap::fibonacci_heap<Job>())
{

}

JobFactory::~JobFactory()
{

}

bool JobFactory::requestJob(/**/)
{
	return true;
}

bool JobFactory::cancelJob(/**/)
{
	return true;
}

bool JobFactory::queryJob(/**/)
{
	return true;
}