#pragma once
#include <BWAPI.h>

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

	/*
		Temporary barracks building implementation
	*/
	//
	bool buildingPhase;
	//

	bool barracksRequested;
	bool barracksBuilt;

	void initialize();
	bool shouldBuildBarracks();
	bool constructBarracks(BWAPI::Unit builder);
	BWAPI::Unit getClosestBuilder(BWAPI::Unitset::iterator depot);

	/*
		VALID OPERATIONS

		Below are all the calls that can be accessed by a job request
	*/

	// A move's success doesn't guarantee that the move was completed, just that it was properly executed
	bool moveT(BWAPI::Unit u, BWAPI::TilePosition t);

	// Position/Unit type relative unit selection
	BWAPI::Unit* getClosestUnit(BWAPI::UnitType type, BWAPI::TilePosition t);
};
