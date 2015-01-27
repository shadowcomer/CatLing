#pragma once
#include <BWAPI.h>
#include <iostream>

#include <assert.h>

/*
	CatLing AI
*/
class CatlingModule : public BWAPI::AIModule
{
public:
  // Virtual functions for callbacks, leave these as they are.
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit unit);
  // Everything below this line is safe to modify.

  CatlingModule();
  ~CatlingModule();

private:
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

	/*
		Temporary building implementation variables
	*/
	unsigned int m_SCVcount;

	bool m_barracksRequested;
	bool m_barracksBuilt;

	bool m_supplyRequested;
	bool m_supplyAttempted;

	BWAPI::TilePosition m_posCommand;

	void initialize();

	/*
		VALID OPERATIONS

		Below are all the calls that can be accessed by a job request
	*/

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
};
