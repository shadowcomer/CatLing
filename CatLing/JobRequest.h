#ifndef JOBREQUEST_H
#define JOBREQUEST_H

enum REQUEST_STATE
{
	INVALID,
	NEXT_UP,
	VALIDATED,
	WAITING
};

class JobRequest
{
public:

private:
	REQUEST_STATE status;

};

#endif