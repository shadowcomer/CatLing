/*
    The ClientLink is the joining piece between Starcraft's process and
    the AI.

    Only a single process can take control of a player controller. Thus,
    to be able to work with multiple modules, the AI process is threaded.

    The ClientLink receives updates from the Broodwar client, after which
    they are processed one by one.

    The ClientLink also controls the creation and destruction of the different
    modules of the AI.
*/

#ifndef CLIENTLINK_H
#define CLIENTLINK_H

#include <BWAPI.h>

#include "../include/TBB/tbb/concurrent_queue.h"

#include "TaskManager.h"
#include "Commander.h"
#include "MicroManager.h"
#include "MacroManager.h"

#include "Task.h"

int runCatling();

class ClientLink
{
public:
    ClientLink();
    ~ClientLink();

    /*
    Safely stops the AI.
    */
    void terminate();

    /* TEMPORARY PUBLIC FUNCTIONALITY */

    /*
    Loads a single module.
    */
    Module* loadModule(ModuleType type);

    /*
    Unloads a single module.
    */
    bool unloadModule(ModuleType type);

    /*
    Process every event individually.
    The event list will be empty if the process hasn't
    called for an update.
    */
    void processEvents();

    /* Possible events from the Starcraft client process. */

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

    bool hasEnoughResources(BWAPI::UnitType type);
    bool hasEnoughSupply(BWAPI::UnitType type);

private:
    /* TEMPORARY PRIVATE FUNCTIONALITY*/
    BWAPI::Player self;

    // Store a reference to as many modules as we have module types
    Module* m_modules[ModuleType::_END];
    TaskManager m_taskManager;
    Executer m_executer;

    SlabAllocator* m_allocator;
    
    long m_totalExecTasks;

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

    bool m_shouldTerminate;

    BWAPI::TilePosition m_posCommand;

    void waitForTermination();

    /*
    Initial configuration call. This method is to be used
    at the beginning of the onStart event.
    */
    void configOnStart();

    /*
    Initializes m_allocator with the Slabs that are going to
    be used by the AI.
    */
    void initializeSlabs();

};

#endif