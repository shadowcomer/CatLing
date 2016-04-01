#include "ResourceRequestAnswer.h"


ResourceRequestAnswer::ResourceRequestAnswer
    (ResourceRequest original):
    originalRequest(original),
    conceded(false),
    missingMinerals(0),
    missingGas(0),
    missingUnits(std::vector<UnitResource>()) {

}


ResourceRequestAnswer::~ResourceRequestAnswer() {

}
