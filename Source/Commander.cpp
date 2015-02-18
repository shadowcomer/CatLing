#include "Commander.h"

using namespace BWAPI;

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
		Unitset units = Broodwar->self()->getUnits();
		for (auto u : units)
		{
			if (!u->exists())
				continue;

			// Ignore the unit if it has one of the following status ailments
			if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
				continue;

			// Ignore the unit if it is in one of the following states
			if (u->isLoaded() || !u->isPowered() || u->isStuck())
				continue;

			// Ignore the unit if it is incomplete or busy constructing
			if (!u->isCompleted() || u->isConstructing())
				continue;

			UnitType type = u->getType();
			if (type.isWorker())
			{
				if (u->isIdle())
				{
					Unit closestPatch = u->getClosestUnit(Filter::GetType == UnitTypes::Resource_Mineral_Field);
					m->m_tasker.requestTask(TGather(u, closestPatch, false));
				}
			}
		}
		tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)0.25));
	}

	std::cout << "Finished Commander loop." << std::endl;
}

Tasker& Commander::tasker()
{
	return m_tasker;
}