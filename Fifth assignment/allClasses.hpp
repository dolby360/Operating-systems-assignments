#ifndef ALL_CLASSES_HPP
#define ALL_CLASSES_HPP

#include "defs.h"

class Task
{
	protected:
		SafeArray *result;
	public:
        //pure virtual class to make this class abstract.
		virtual void Action(void *arg) = 0;
		virtual ~Task() = 0;
};

#endif