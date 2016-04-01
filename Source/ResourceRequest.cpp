#include "ResourceRequest.h"


ResourceRequest::ResourceRequest() :
minerals(0),
gas(0),
units(std::vector<UnitResource>())
{
}


ResourceRequest::~ResourceRequest()
{
}
