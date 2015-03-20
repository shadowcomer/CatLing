#include "Module.h"

Module::Module(Tasker& tsk):
m_shuttingDown(false),
m_tasker(tsk)
{

}

Module::~Module()
{

}

bool Module::isTerminating()
{
	return m_shuttingDown;
}

bool Module::shutdown()
{
	m_shuttingDown = shutdownHelper();
	m_thread.join();
	return m_shuttingDown;
}

tbb::tbb_thread& Module::getThread()
{
	return m_thread;
}

Tasker& Module::tasker()
{
	return m_tasker;
}