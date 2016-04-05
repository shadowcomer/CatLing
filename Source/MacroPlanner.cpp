#include "MacroPlanner.h"

MacroPlanner::MacroPlanner() {

}

MacroPlanner::~MacroPlanner() {

}

std::unique_ptr<bt::BehaviorTree>
    MacroPlanner::getNextComplexAction() {
    std::unique_ptr<bt::BehaviorTree> plan;

    return std::move(plan);
}
