#ifndef CLIENTLINK_H
#define CLIENTLINK_H

#include <BWAPI.h>
#include <assert.h>

class ClientLink
{
public:
	ClientLink();
	~ClientLink();

	/* TEMPORARY PUBLIC FUNCTIONALITY */

	void processEvents();

	void onStart();
	void onEnd(bool isWinner);
	void onFrame();
	void onSendText(std::string text);
	void onReceiveText(BWAPI::Player player, std::string text);
	void onPlayerLeft(BWAPI::Player player);
	void onNukeDetect(BWAPI::Position target);
	void onUnitDiscover(BWAPI::Unit unit);
	void onUnitEvade(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);
	void onSaveGame(std::string gameName);
	void onUnitComplete(BWAPI::Unit unit);
	
	void spendProjectedCost(BWAPI::UnitType type);

	// A move's success doesn't guarantee that the move was completed, just that it was properly executed
	bool moveToTile(BWAPI::Unit unit, BWAPI::TilePosition position);

	// Position/Unit type relative unit selection
	BWAPI::Unit getClosest(BWAPI::UnitType type, BWAPI::TilePosition t);

	bool build(BWAPI::Unit builder, BWAPI::UnitType type, BWAPI::TilePosition location);
	bool train(BWAPI::Unit trainer, BWAPI::UnitType type);


	/// Determines whether a given builder can build another specific unit
	bool unitCanBuild(BWAPI::Unit builder, BWAPI::UnitType type);
	bool unitCanTrain(BWAPI::Unit trainer, BWAPI::UnitType type);
	bool hasEnoughResources(BWAPI::UnitType type);
	bool hasEnoughSupply(BWAPI::UnitType type);

private:
	/* TEMPORARY PRIVATE FUNCTIONALITY*/
	BWAPI::Player self;

	// Map dimensions in Build Tile, Walk Tile and Position
	int m_mapWidth_BT;
	int m_mapHeight_BT;

	int m_mapWidth_WT;
	int m_mapHeight_WT;

	int m_mapWidth_P;
	int m_mapHeight_P;

	int m_projectedMinerals;
	int m_projectedGas;

	unsigned int m_SCVcount;

	bool m_barracksRequested;
	bool m_barracksBuilt;

	bool m_supplyRequested;
	bool m_supplyAttempted;

	BWAPI::TilePosition m_posCommand;
};

#endif