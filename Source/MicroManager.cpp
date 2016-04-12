#include "MicroManager.h"


MicroManager::MicroManager(Tasker& tsk, Module** modules,
    SlabAllocator* alloc) :
    Module(tsk, modules, alloc) {

}

MicroManager::~MicroManager()
{

}

void MicroManager::launch()
{
    m_thread = tbb::tbb_thread(&MicroManager::run, this);
}

bool MicroManager::shutdownHelper()
{
    bool result = true;
    return result;
}

void MicroManager::run(MicroManager *m)
{

    m->notifyShutdownCompletion();
}