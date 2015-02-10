#include "Commander.h"

Commander::Commander()
{

}

Commander::~Commander()
{

}

bool Commander::linkClient(ClientLink * cli)
{
	if (nullptr == cli)
	{
		return false;
	}
	m_client = cli;
	return true;
}

void Commander::execute(int id)
{
	while (true)
	{
		m_client->requestAction(id);
		tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)0.25));
	}
}