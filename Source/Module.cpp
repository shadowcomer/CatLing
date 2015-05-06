#include "Module.h"

Module::Module():
m_shuttingDown(false)
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
	return (m_shuttingDown = shutdownHelper());
}

int Module::getFramesToWake(){
	return framesToWake;
}

tbb::tbb_thread& Module::getThread()
{
	return m_thread;
}