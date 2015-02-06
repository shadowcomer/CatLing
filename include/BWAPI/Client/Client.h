#pragma once
#include "GameData.h"
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "GameTable.h"

#include "../WindowsTypes.h"


namespace BWAPI
{
  class Client
  {
  public:
    Client();
    ~Client();

    bool isConnected() const;
    bool connect();
    void disconnect();
    void update();

    GameData  *data;
  private:
    HANDLE    pipeObjectHandle;
    HANDLE    mapFileHandle;
    HANDLE    gameTableFileHandle;
    GameTable *gameTable;
    
    bool connected;
  };
  extern Client BWAPIClient;
}
