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
	// TODO: WARNING! This is unsafe! The caller could wait indefinitely if the shutdownHelper reports a failure.
	// It's unclear how to fix this, because we have to guarantee that the thread shuts down properly before we can leave, so we
	// don't lose any data.
	m_shuttingDown = shutdownHelper();
	m_thread.join();
	return m_shuttingDown;
}

int Module::getFramesToWake(){
	return framesToWake;
}

tbb::tbb_thread& Module::getThread()
{
	return m_thread;
}

Tasker& Module::tasker()
{
	return m_tasker;
}

bool Module::setAllocator(SlabAllocator* allocator)
{
	if (allocator == nullptr)
		return false;

	// We shouldn't be able to change allocators if there's already an active one.
	if (m_allocator != nullptr)
		return false;

	m_allocator = allocator;
	return true;
}