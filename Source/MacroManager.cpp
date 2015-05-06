#include "MacroManager.h"

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk) :
Module(),
m_tasker(tsk)
{

}

MacroManager::~MacroManager()
{

}

void MacroManager::launch()
{
	m_thread = tbb::tbb_thread(&MacroManager::run, this);
}

bool MacroManager::shutdownHelper()
{
	bool result = true;
	return result;
}

void MacroManager::run(MacroManager *m)
{
	
}