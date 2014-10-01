// CatLing.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CatlingModule.h"

#include <iostream>

using namespace BWAPI;
using namespace Filter;

void bindCall(std::tr1::function<void(BWAPI::Game*)> call);
void cancel(BWAPI::Game* g);

void CatlingModule::initialize()
{
	self = Broodwar->self();
	m_barracksRequested = false;
	m_barracksBuilt = false;

	m_supplyRequested = false;
	m_supplyAttempted = false;

	m_mapWidth_BT = Broodwar->mapWidth();
	m_mapHeight_BT = Broodwar->mapHeight();

	m_mapWidth_WT = m_mapWidth_BT * 4;
	m_mapHeight_WT = m_mapHeight_BT * 4;

	m_mapWidth_P = m_mapWidth_BT * TILE_SIZE;
	m_mapHeight_P = m_mapHeight_BT * TILE_SIZE;

	m_SCVcount = 0;
	m_projectedMinerals = 0;
	m_projectedGas = 0;
}

void CatlingModule::onStart()
{
	// Hello World!
	Broodwar->sendText("Hello world!");
	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(1);

	// Check if this is a replay
	// NOTE: Having replay watching functionality, we could set up the bot to study replays.
	//			Replays could be it's own, other bot's, or a human's.
	if (Broodwar->isReplay())
	{
		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for(auto p = players.begin(); p != players.end(); ++p)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) // First make sure there is an enemy
			Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
		
		// Call for this class' initialization (can't call a constructor because we're injecting)
		initialize();
	}

}

void CatlingModule::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

void CatlingModule::onFrame()
{
	// Called once every game frame
	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 40, "SCVs: %d", m_SCVcount);
	if(m_barracksRequested)
		Broodwar->drawTextScreen(200, 60, "Building barracks...");
	if(m_supplyRequested)
		Broodwar->drawTextScreen(200, 80, "Building supply depot...");

	// Return if the game is a replay or is paused
	if(Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if(Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	// Iterate through all the units that we own
	Unitset myUnits = Broodwar->self()->getUnits();
	for(Unitset::iterator u = myUnits.begin(); u != myUnits.end(); ++u)
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if(!u->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if(u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if(u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if(!u->isCompleted() || u->isConstructing())
			continue;

		UnitType type = u->getType();
		if(type.isResourceDepot())
		{
			// Build workers if needed
			if(u->isIdle() && m_SCVcount < 24)
				train(*u, UnitTypes::Terran_SCV);

			// Build barracks if we don't have one
			if(m_SCVcount > 8 && !m_barracksRequested && !m_barracksBuilt)
			{
				UnitType barracksType = UnitTypes::Terran_Barracks;
				Unit builder = u->getClosestUnit(GetType == barracksType.whatBuilds().first && IsIdle || IsGatheringMinerals);
				TilePosition location = Broodwar->getBuildLocation(barracksType, u->getTilePosition(), 80);

				m_barracksRequested = build(builder, barracksType, location);
			}

			// Build supply if we need
			if((float)self->supplyUsed() / (float)self->supplyTotal() >= 0.7f &&
				!m_supplyRequested)
			{
				UnitType supplyType = UnitTypes::Terran_Supply_Depot;
				Unit builder = u->getClosestUnit(GetType == supplyType.whatBuilds().first && (IsIdle || IsGatheringMinerals));
				TilePosition location = Broodwar->getBuildLocation(supplyType, builder->getTilePosition(), 100);
				
				m_supplyRequested = build(builder, supplyType, location);
			}

		} else if(UnitTypes::Terran_Barracks == type)
		{
			if(!u->exists())
				Broodwar << "Barracks doesn't exist but it's here..." << std::endl;
			// Train marines
			if(u->isIdle())
				if(train(*u, UnitTypes::Terran_Marine))
					Broodwar << "Training marine" << std::endl;

		} else if(UnitTypes::Terran_SCV == type)
		{
			if(u->isIdle())
			{
				Unit closestPatch = u->getClosestUnit(GetType == UnitTypes::Resource_Mineral_Field);
				u->gather(closestPatch);
			}
		}

	}
}

void CatlingModule::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void CatlingModule::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void CatlingModule::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void CatlingModule::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if(target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else 
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void CatlingModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void CatlingModule::onUnitEvade(BWAPI::Unit unit)
{
}

void CatlingModule::onUnitShow(BWAPI::Unit unit)
{
}

void CatlingModule::onUnitHide(BWAPI::Unit unit)
{
}

void CatlingModule::onUnitCreate(BWAPI::Unit unit)
{
	UnitType type = unit->getType();
	
	// Keep projected resources consistency
	m_projectedMinerals -= type.mineralPrice();
	m_projectedGas -= type.gasPrice();

	Broodwar << "Created " << type.toString() << std::endl;
}

void CatlingModule::onUnitDestroy(BWAPI::Unit unit)
{
	UnitType type = unit->getType();
	if(UnitTypes::Terran_SCV == type)
		m_SCVcount = m_SCVcount > 0 ? m_SCVcount - 1 : 0;
	if(UnitTypes::Terran_Bunker == type)
		m_barracksBuilt = false;
}

void CatlingModule::onUnitMorph(BWAPI::Unit unit)
{
	if(Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if(unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount()/24;
			int minutes = seconds/60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void CatlingModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void CatlingModule::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void CatlingModule::onUnitComplete(BWAPI::Unit unit)
{
	UnitType type = unit->getType();
	if(UnitTypes::Terran_Barracks == type)
		{ m_barracksBuilt = !(m_barracksRequested = false); Broodwar << "Barracks built" << std::endl; }
	else if(UnitTypes::Terran_SCV == type)
		m_SCVcount++;
	else if(UnitTypes::Terran_Supply_Depot == type)
		m_supplyRequested = false;
}

bool CatlingModule::moveToTile(Unit unit, TilePosition position)
{
	assert(unit != nullptr && position.isValid());
	return unit->move(Position(position),false);
}

void cancel(BWAPI::Game* g)
{
	Broodwar << "Building was stopped! OOOPS!" << std::endl;
}

bool CatlingModule::build(Unit builder, UnitType type, TilePosition location)
{
	bool success = false;
	assert(unitCanBuild(builder,type));

	if(success = builder->build(type, location))
		spendProjectedCost(type);

	// Check until the building was stopped
	// DUMMY: Failed construction callback. Current implementation is just for learning how it works.
	Broodwar->registerEvent([](BWAPI::Game*) -> void {Broodwar << "Building was stopped! OOOPS!" << std::endl;},
							[](BWAPI::Game*) -> bool {return true;},
							1,
							1);
	return success;
}

bool CatlingModule::unitCanBuild(Unit builder, UnitType type)
	{ return builder != nullptr && Broodwar->canMake(type, builder); }

bool CatlingModule::train(Unit trainer, UnitType type)
{
	bool success = false;
	// Check params
	assert(unitCanTrain(trainer,type));
	// Check resources
	if(!hasEnoughSupply(type) || !hasEnoughResources(type))
		return false;

	if(success = trainer->train(type))
		spendProjectedCost(type);
	return success;
}

bool CatlingModule::hasEnoughSupply(BWAPI::UnitType type)
	{ return self->supplyTotal() >= (self->supplyUsed() + type.supplyRequired()); }

bool CatlingModule::hasEnoughResources(UnitType type)
	{ return (self->minerals() - m_projectedMinerals) >= type.mineralPrice() && (self->gas() - m_projectedGas) >= type.gasPrice(); }

void CatlingModule::spendProjectedCost(UnitType type)
{
	m_projectedMinerals += type.mineralPrice();
	m_projectedGas += type.gasPrice();
}

bool CatlingModule::unitCanTrain(Unit trainer, UnitType type)
	{ return trainer && Broodwar->canMake(type, trainer); }

CatlingModule::CatlingModule() { }
CatlingModule::~CatlingModule() { }