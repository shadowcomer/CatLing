#include "Commander.h"

Commander::Commander(Tasker& tsk) :
Module(),
m_tasker(tsk)
{

}

Commander::~Commander()
{

}

void Commander::launch()
{
	m_thread = tbb::tbb_thread(&Commander::run, this);
}

bool Commander::shutdownHelper()
{

	return true;
}

void Commander::run(Commander* m)
{
	std::cout << "Started Commander loop." << std::endl;
	while (!m->isTerminating())
	{
		m->tasker().requestTask();
		tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)0.25));
	}

	std::cout << "Finished Commander loop." << std::endl;
}

Tasker& Commander::tasker()
{
	return m_tasker;
}