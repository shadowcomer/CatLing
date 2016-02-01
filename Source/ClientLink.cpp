#include <BWAPI/Client.h>

#include <WinSock2.h>
#include <Windows.h>
#include <mongo\bson\bson.h>
#include <mongo\client\dbclient.h>
#include <mongo\client\init.h>

#include <strsafe.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>

#include "ClientLink.h"


using namespace BWAPI;

/*
################################################################
################################################################
                    Connection definitions
################################################################
################################################################
*/

void drawStats();
void drawBullets();
void drawVisibilityData();
void showPlayers();
void showForces();
bool show_bullets;
bool show_visibility_data;

void reconnect()
{
    while (!BWAPIClient.connect())
    {
        //std::cout << GetLastError();
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
    }
}

int runCatling()
{
    std::cout << "Initializing Database..." << std::endl;
    mongo::client::initialize();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    TCHAR lpszClientPath[500] =
        TEXT(".\\db\\mongod.exe --dbpath=.\\data\\db");

    int created = CreateProcess(
        NULL,
        lpszClientPath,
        NULL, NULL, false,
        0, NULL, NULL,
        &si, &pi);

    if (created != 0){
        std::cout << "Started mongo daemon." << std::endl;
    }
    else {
        std::cout << "Error starting mongo. Exiting." << std::endl;
        std::cout << GetLastError() << std::endl;
        exit(1);
    }

    mongo::DBClientConnection c;
    try {
        c.connect("localhost");
        std::cout << "Connected to mongo database." << std::endl;
    }
    catch (const mongo::DBException &e){
        std::cout << "MongoDB Exception: " << e.what() << std::endl;
    }

    ClientLink link;
    std::cout << "Waiting to connect to SC Client..." << std::endl;
    //std::cout << BWAPI::BWAPIClient.isConnected() << std::endl;
    reconnect();
    while (true)
    {
        std::cout << "waiting to enter match" << std::endl;
        while (!Broodwar->isInGame())
        {

            BWAPI::BWAPIClient.update();
            if (!BWAPI::BWAPIClient.isConnected())
            {
                std::cout << "Reconnecting..." << std::endl;
                reconnect();
            }
        }

        std::cout << "starting match!" << std::endl;
        Broodwar << "The map is " << Broodwar->mapName() << ", a " << Broodwar->getStartLocations().size() << " player map" << std::endl;
        // Enable some cheat flags
        Broodwar->enableFlag(Flag::UserInput);
        // Uncomment to enable complete map information
        //Broodwar->enableFlag(Flag::CompleteMapInformation);

        show_bullets = false;
        show_visibility_data = false;

        if (Broodwar->isReplay())
        {
            Broodwar << "The following players are in this replay:" << std::endl;
            Playerset players = Broodwar->getPlayers();
            for (auto p : players)
            {
                if (!p->getUnits().empty() && !p->isNeutral())
                    Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
            }
        }
        else
        {
            if (Broodwar->enemy())
                Broodwar << "The match up is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
        }

        Module* loadedModule = nullptr;
        loadedModule = link.loadModule(ModuleType::COMMANDER);
        assert(nullptr != loadedModule);

        loadedModule = link.loadModule(ModuleType::MICROMGR);
        assert(nullptr != loadedModule);

        loadedModule = link.loadModule(ModuleType::MACROMGR);
        assert(nullptr != loadedModule);

        while (Broodwar->isInGame())
        {
            link.processEvents();

            if (show_bullets)
                drawBullets();

            if (show_visibility_data)
                drawVisibilityData();

            drawStats();
            Broodwar->drawTextScreen(300, 0, "FPS: %f", Broodwar->getAverageFPS());

            BWAPI::BWAPIClient.update();
            if (!BWAPI::BWAPIClient.isConnected())
            {
                std::cout << "Reconnecting..." << std::endl;
                reconnect();
            }
        }
        std::cout << "Game ended" << std::endl;
    }

    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
    mongo::client::shutdown();
    return 0;
}

void drawStats()
{
    int line = 0;
    Broodwar->drawTextScreen(5, 0, "I have %d units:", Broodwar->self()->allUnitCount());
    for (auto& unitType : UnitTypes::allUnitTypes())
    {
        int count = Broodwar->self()->allUnitCount(unitType);
        if (count)
        {
            Broodwar->drawTextScreen(5, 16 * line, "- %d %s%c", count, unitType.c_str(), count == 1 ? ' ' : 's');
            ++line;
        }
    }
}

void drawBullets()
{
    for (auto &b : Broodwar->getBullets())
    {
        Position p = b->getPosition();
        double velocityX = b->getVelocityX();
        double velocityY = b->getVelocityY();
        Broodwar->drawLineMap(p, p + Position((int)velocityX, (int)velocityY), b->getPlayer() == Broodwar->self() ? Colors::Green : Colors::Red);
        Broodwar->drawTextMap(p, "%c%s", b->getPlayer() == Broodwar->self() ? Text::Green : Text::Red, b->getType().c_str());
    }
}

void drawVisibilityData()
{
    int wid = Broodwar->mapHeight(), hgt = Broodwar->mapWidth();
    for (int x = 0; x < wid; ++x)
        for (int y = 0; y < hgt; ++y)
        {
            if (Broodwar->isExplored(x, y))
                Broodwar->drawDotMap(x * 32 + 16, y * 32 + 16, Broodwar->isVisible(x, y) ? Colors::Green : Colors::Blue);
            else
                Broodwar->drawDotMap(x * 32 + 16, y * 32 + 16, Colors::Red);
        }
}

void showPlayers()
{
    Playerset players = Broodwar->getPlayers();
    for (auto p : players)
        Broodwar << "Player [" << p->getID() << "]: " << p->getName() << " is in force: " << p->getForce()->getName() << std::endl;
}

void showForces()
{
    Forceset forces = Broodwar->getForces();
    for (auto f : forces)
    {
        Playerset players = f->getPlayers();
        Broodwar << "Force " << f->getName() << " has the following players:" << std::endl;
        for (auto p : players)
            Broodwar << "  - Player [" << p->getID() << "]: " << p->getName() << std::endl;
    }
}

/*
################################################################
################################################################
                    ClientLink definitions
################################################################
################################################################
*/

ClientLink::ClientLink() :
m_shouldTerminate(false),
m_barracksRequested(false),
m_barracksBuilt(false),
m_supplyRequested(false),
m_supplyAttempted(false),
m_SCVcount(0),
m_projectedMinerals(0),
m_projectedGas(0),
m_totalExecTasks(0),
m_executer(m_taskManager.getOutputInterface())
{
    for (unsigned int i = ModuleType::COMMANDER; i < ModuleType::_END; i++)
    {
        m_modules[i] = nullptr;
    }

    // TODO: Change to stack allocation
    m_allocator = new SlabAllocator();
}

ClientLink::~ClientLink()
{
    delete m_allocator;
}

Module* ClientLink::loadModule(ModuleType type)
{
    // Skip if it's the _END special type
    if (type == ModuleType::_END)
    {
        return nullptr;
    }

    // Skip if it's already loaded
    if(m_modules[type] != nullptr)
    {
        return nullptr;
    }

    Module* tmp = nullptr;

    switch (type)
    {
    case ModuleType::COMMANDER:
        tmp = new Commander(m_taskManager.getInputInterface());
        break;
    case ModuleType::MACROMGR:
        tmp = new MacroManager(m_taskManager.getInputInterface());
        break;
    case ModuleType::MICROMGR:
        tmp = new MicroManager(m_taskManager.getInputInterface());
        break;
    default:
        return nullptr;
        break;
    }

    bool success = tmp->setAllocator(m_allocator);
    if (!success){
        return nullptr;
    }

    m_modules[type] = tmp;
    m_modules[type]->launch();

    return m_modules[type];
}

bool ClientLink::unloadModule(ModuleType type)
{
    if (type != ModuleType::_END && m_modules[type] != nullptr)
    {
        m_modules[type]->shutdown();
        delete m_modules[type];
        m_modules[type] = nullptr;
        return true;
    }

    return false;
}

void ClientLink::terminate()
{
    // TODO: Update to take a ModuleType instead of doing an int conversion.
    for (int i = 0; i < ModuleType::_END; ++i)
    {
        unloadModule((ModuleType)i);
    }

    //waitForTermination();
}

void ClientLink::waitForTermination()
{
    for (int i = 0; i < ModuleType::_END; ++i)
    {
        m_modules[i]->getThread().join();
    }
}


void ClientLink::processEvents()
{
    for (auto &e : Broodwar->getEvents())
    {
        switch (e.getType())
        {
        case EventType::UnitDiscover:
            onUnitDiscover(e.getUnit());
            break;
        case EventType::UnitShow:
            onUnitShow(e.getUnit());
            break;
        case EventType::UnitHide:
            onUnitHide(e.getUnit());
            break;
        case EventType::UnitEvade:
            onUnitEvade(e.getUnit());
            break;
        case EventType::UnitMorph:
            onUnitMorph(e.getUnit());
            break;
        case EventType::UnitRenegade:
            onUnitRenegade(e.getUnit());
            break;
        case EventType::UnitCreate:
            onUnitCreate(e.getUnit());
            break;
        case EventType::UnitComplete:
            onUnitComplete(e.getUnit());
            break;
        case EventType::UnitDestroy:
            onUnitDestroy(e.getUnit());
            break;
        case EventType::MatchFrame:
            // Wake up each module that should execute during this frame.
            for each (Module* m in m_modules)
            {	
                if (m != nullptr)
                {
                    int currentFrame = Broodwar->getFrameCount();
                    int elapsedFrames = currentFrame - m->lastExecFrame();
                    if (elapsedFrames >= m->getFrameExecDelta()){
                        m->resumeExecution();
                    }
                }
            }

            onFrame();
            break;
        case EventType::NukeDetect:
            onNukeDetect(e.getPosition());
            break;
        case EventType::SendText:
            onSendText(e.getText());
            break;
        case EventType::ReceiveText:
            onReceiveText(e.getPlayer(), e.getText());
            break;
        case EventType::PlayerLeft:
            onPlayerLeft(e.getPlayer());
            break;
        case EventType::MatchStart:
            onStart();
            break;
        case EventType::MatchEnd:
            onEnd(e.isWinner());
            break;
        case EventType::SaveGame:
            onSaveGame(e.getText());
            break;

        case EventType::None:
            break;
        default:
            break;
        }
    }
}

bool ClientLink::moveToTile(Unit unit, TilePosition position)
{
    assert(unit != nullptr && position.isValid());
    return unit->move(Position(position), false);
}

bool ClientLink::hasEnoughSupply(BWAPI::UnitType type)
{
    return self->supplyTotal() >= (self->supplyUsed() + type.supplyRequired());
}

bool ClientLink::hasEnoughResources(BWAPI::UnitType type)
{
    return (self->minerals() - m_projectedMinerals) >= type.mineralPrice() && (self->gas() - m_projectedGas) >= type.gasPrice();
}

void ClientLink::spendProjectedCost(BWAPI::UnitType type)
{
    m_projectedMinerals += type.mineralPrice();
    m_projectedGas += type.gasPrice();
}


void ClientLink::onStart()
{
    configOnStart();
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
        for (auto p : players)
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
    }

}

void ClientLink::onEnd(bool isWinner)
{
    terminate();
    // Called when the game ends
    if (isWinner)
    {
        // Log your win here!
    }
}

void ClientLink::onFrame()
{

    Broodwar->drawTextScreen(200, 0, "Task Count: %d", m_totalExecTasks);

    if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
        return;

    if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
        return;

    m_totalExecTasks = m_executer.executeAllTasks();

    /*
    // Called once every game frame
    // Display the game frame rate as text in the upper left area of the screen
    Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
    Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
    Broodwar->drawTextScreen(200, 40, "SCVs: %d", m_SCVcount);
    if (m_barracksRequested)
        Broodwar->drawTextScreen(200, 60, "Building barracks...");
    if (m_supplyRequested)
        Broodwar->drawTextScreen(200, 80, "Building supply depot...");

    // Return if the game is a replay or is paused
    if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
        return;

    // Prevent spamming by only running our onFrame once every number of latency frames.
    // Latency frames are the number of frames before commands are processed.
    if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
        return;

    // Build supply if we need
    if ((float)self->supplyUsed() / (float)self->supplyTotal() >= 0.7f &&
        !m_supplyRequested)
    {
        UnitType supplyType = UnitTypes::Terran_Supply_Depot;
        Unitset set = Broodwar->getUnitsInRadius((Position)m_posCommand / 32, 20000, Filter::GetType == UnitTypes::Terran_SCV &&
            Filter::IsIdle || Filter::IsGatheringMinerals);
        if (!set.empty())
        {
            Unit builder = *set.begin();
            TilePosition location = Broodwar->getBuildLocation(supplyType, builder->getTilePosition(), 100);
            m_supplyRequested = build(builder, supplyType, location);
        }
        else
            Broodwar << "Unitset is EMPTY!" << std::endl;
    }

    // Iterate through all the units that we own
    Unitset myUnits = Broodwar->self()->getUnits();
    for (auto u : myUnits)
    {
        // Ignore the unit if it no longer exists
        // Make sure to include this block when handling any Unit pointer!
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
        if (type.isResourceDepot())
        {
            // Build workers if needed
            if (u->isIdle() && m_SCVcount < 24 && unitCanTrain(u, UnitTypes::Terran_SCV))
                train(u, UnitTypes::Terran_SCV);

            // Build barracks if we don't have one
            if (m_SCVcount > 8 && !m_barracksRequested && !m_barracksBuilt)
            {
                UnitType barracksType = UnitTypes::Terran_Barracks;
                Unit builder = u->getClosestUnit(Filter::GetType == barracksType.whatBuilds().first && Filter::IsIdle || Filter::IsGatheringMinerals);
                TilePosition location = Broodwar->getBuildLocation(barracksType, u->getTilePosition(), 80);

                m_barracksRequested = build(builder, barracksType, location);
            }

        }
        else if (UnitTypes::Terran_Barracks == type)
        {
            if (!u->exists())
                Broodwar << "Barracks doesn't exist but it's here..." << std::endl;
            // Train marines
            if (u->isIdle())
                if (train(u, UnitTypes::Terran_Marine))
                    Broodwar << "Training marine" << std::endl;

        }
        else if (UnitTypes::Terran_SCV == type)
        {
            if (u->isIdle())
            {
                Unit closestPatch = u->getClosestUnit(Filter::GetType == UnitTypes::Resource_Mineral_Field);
                u->gather(closestPatch);
            }
        }

    }

    */
}

void ClientLink::onSendText(std::string text)
{

    // Send the text to the game if it is not being processed.
    Broodwar->sendText("%s", text.c_str());

    // Make sure to use %s and pass the text as a parameter,
    // otherwise you may run into problems when you use the %(percent) character!

}

void ClientLink::onReceiveText(BWAPI::Player player, std::string text)
{
    // Parse the received text
    Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ClientLink::onPlayerLeft(BWAPI::Player player)
{
    // Interact verbally with the other players in the game by
    // announcing that the other player has left.
    Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ClientLink::onNukeDetect(BWAPI::Position target)
{

    // Check if the target is a valid position
    if (target)
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

void ClientLink::onUnitDiscover(BWAPI::Unit unit)
{
}

void ClientLink::onUnitEvade(BWAPI::Unit unit)
{
}

void ClientLink::onUnitShow(BWAPI::Unit unit)
{
}

void ClientLink::onUnitHide(BWAPI::Unit unit)
{
}

void ClientLink::onUnitCreate(BWAPI::Unit unit)
{
    // WARNING: It's possible that this function is registering units
    // that are not our own.
    BWAPI::UnitType type = unit->getType();

    // Keep projected resources consistency
    m_projectedMinerals -= type.mineralPrice();
    m_projectedGas -= type.gasPrice();

    Broodwar << "Created " << type.toString() << std::endl;
}

void ClientLink::onUnitDestroy(BWAPI::Unit unit)
{
    // FIX: This function could be registering another team's units deaths.
    BWAPI::UnitType type = unit->getType();
    if (UnitTypes::Terran_SCV == type)
        m_SCVcount = m_SCVcount > 0 ? m_SCVcount - 1 : 0;
    if (UnitTypes::Terran_Bunker == type)
        m_barracksBuilt = false;
}

void ClientLink::onUnitMorph(BWAPI::Unit unit)
{
    if (Broodwar->isReplay())
    {
        // if we are in a replay, then we will print out the build order of the structures
        if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
        {
            int seconds = Broodwar->getFrameCount() / 24;
            int minutes = seconds / 60;
            seconds %= 60;
            Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
        }
    }
}

void ClientLink::onUnitRenegade(BWAPI::Unit unit)
{
}

void ClientLink::onSaveGame(std::string gameName)
{
    Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ClientLink::onUnitComplete(BWAPI::Unit unit)
{
    // FIX: This function could be registering another team's finished builds.
    BWAPI::UnitType type = unit->getType();
    if (UnitTypes::Terran_Barracks == type)
    {
        m_barracksBuilt = !(m_barracksRequested = false); Broodwar << "Barracks built" << std::endl;
    }
    else if (UnitTypes::Terran_SCV == type)
        m_SCVcount++;
    else if (UnitTypes::Terran_Supply_Depot == type)
        m_supplyRequested = false;
}

void ClientLink::configOnStart()
{
    self = Broodwar->self();
    m_posCommand = self->getStartLocation();
    m_mapWidth_BT = Broodwar->mapWidth();
    m_mapHeight_BT = Broodwar->mapHeight();
    m_mapWidth_WT = m_mapWidth_BT * 4;
    m_mapHeight_WT = m_mapHeight_BT * 4;

    m_mapWidth_P = m_mapWidth_BT * TILE_SIZE;
    m_mapHeight_P = m_mapHeight_BT * TILE_SIZE;
}
