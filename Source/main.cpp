#include <BWAPI.h>
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
#include "Commander.h"

using namespace BWAPI;


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

int main(int argc, const char* argv[])
{
    std::cout << "Starting..." << std::endl;
    std::cout << "Initializing Database..." << std::endl;
    mongo::client::initialize();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    TCHAR lpszClientPath[500] = TEXT("./db/mongod.exe --dbpath=.\\data\\db\\");
    CreateProcess(NULL, lpszClientPath, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
    mongo::DBClientConnection c;
    c.connect("localhost");

    std::cout << "Database initialized." << std::endl;

    //mongo::BSONObj p = BSON("prueba" << "Si");
    //c.insert("macro.prueba",p);
    //c.insert("micro.prueba",p);

    ClientLink link;
    std::cout << "Connecting..." << std::endl;
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
        Broodwar->sendText("Hello world!");
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

        std::cout << "Loading modules..." << std::endl;
        link.loadModule(ModuleType::COMMANDER);
        link.loadModule(ModuleType::MICROMGR);
        link.loadModule(ModuleType::MACROMGR);

        std::cout << "Modules loaded." << std::endl;

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
