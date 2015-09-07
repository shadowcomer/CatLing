#include "MacroManager.h"

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk) :
Module(tsk)
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

	return true;
}

void MacroManager::run(MacroManager* m)
{
	std::cout << "Started MacroManager loop." << std::endl;
	Unitset units = Broodwar->self()->getUnits();

	for (auto u : units)
	{
		if (u->getType().isResourceDepot())
		{
			m->m_command = u;
			break;
		}
	}

	Slab* resTable = nullptr;
	std::cout << "MacroManager waiting for 'resources' to be created." << std::endl;

	while(!m->m_allocator->find("resources", &resTable))
	{
		tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)1));
	}

	std::cout << "MacroManager found 'resources' table." << std::endl;
	std::cout << "Fields in 'resources' table: " << std::endl;
	TypeList const fields = resTable->discover();
	for each(auto f in fields)
	{
		Broodwar << "\t- " << f.first << std::endl;
	}

	while(!m->isTerminating())
	{
		// When we have plenty of minerals, build a barracks
		if(Broodwar->self()->minerals() > 200)
		{
			// Find a builder
			Unitset set = Broodwar->getUnitsInRadius(
				(Position)Broodwar->self()->getStartLocation() / 32,
				20000,
				Filter::GetType == UnitTypes::Terran_SCV &&
				Filter::IsIdle || Filter::IsGatheringMinerals);

			if (!set.empty())
			{
				Unit builder = *set.begin();
				TilePosition location = 
					Broodwar->getBuildLocation(UnitTypes::Terran_Barracks,
					Broodwar->self()->getStartLocation(),
					100);
				m->tasker().requestTask(
					new TBuild(builder,
						UnitTypes::Terran_Barracks,
						location));
			}
		}

		// Sleep
		tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)5));
	}

	std::cout << "Finished MacroManager loop." << std::endl;

}
