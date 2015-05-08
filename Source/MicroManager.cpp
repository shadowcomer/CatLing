#include "MicroManager.h"

MicroManager::MicroManager(Tasker& tsk) : Module(tsk)
{

}


MicroManager::~MicroManager()
{

}
void MicroManager::launch()
{
	m_thread = tbb::tbb_thread(&MicroManager::run, this);
}

bool MicroManager::shutdownHelper()
{
	bool result = true;
	return result;
}

void MicroManager::run(MicroManager *m)
{
	std::cout << "Started MicroManager loop." << std::endl;
}