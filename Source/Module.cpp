#include "Module.h"

#include <assert.h>

Module::Module(Tasker& tsk) :
m_shuttingDown(false),
m_shutdown(false),
m_tasker(tsk),
m_allocator(nullptr),
m_shouldWake(false),
m_lastExecFrame(0),
m_frameExecDelta(1)
{

}

Module::~Module()
{

}

bool Module::isShuttingDown()
{
    return m_shuttingDown;
}

bool Module::shutdown()
{
    // TODO: Guarantee there's no deadlock.
    m_shuttingDown = shutdownHelper();
    resumeExecution();

    {
        std::unique_lock<std::mutex> lk(m_shutdownMutex);
        if (!m_shutdown){
            m_workCond.wait(lk);
        }
        // At this point, m_shutdown should always be true
        assert(m_shutdown);
    }

    return m_shuttingDown;
}

void Module::notifyShutdownCompletion() {
    {
        std::unique_lock<std::mutex> lk(m_shutdownMutex);
        m_shutdown = true;
        m_workCond.notify_all();
    }
}

int Module::lastExecFrame(){
    return m_lastExecFrame;
}

void Module::setLastExecFrame(int frame){
    m_lastExecFrame = frame;
}

void Module::setFrameExecDelta(int delta){
    assert(delta > 0);
    m_frameExecDelta = delta;
}

int Module::getFrameExecDelta(){
    return m_frameExecDelta;
}

void Module::sleepExecution(){
    {
        std::unique_lock<std::mutex> lk(m_workMutex);
        // m_shouldWake is there to stop spurious wakes.
        m_workCond.wait(lk, [this]() -> bool { return m_shouldWake; });
        m_shouldWake = false;
    }
}

void Module::resumeExecution(){
    m_shouldWake = true;
    m_workCond.notify_one();
    setLastExecFrame(BWAPI::Broodwar->getFrameCount());
}

tbb::tbb_thread& Module::getThread()
{
    return m_thread;
}

Tasker& Module::tasker()
{
    return m_tasker;
}

bool Module::setAllocator(SlabAllocator* allocator)
{
    if (allocator == nullptr)
        return false;

    // We shouldn't be able to change allocators if there's already an active one.
    if (m_allocator != nullptr)
        return false;

    m_allocator = allocator;
    return true;
}