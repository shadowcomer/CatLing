#ifndef COMMANDER_H
#define COMMANDER_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"

#include "ClientLink.h"


class Commander
{
public:
	Commander();
	~Commander();

	bool linkClient(ClientLink * cli);
	void execute(int id);

private:

	ClientLink * m_client;

};
#endif