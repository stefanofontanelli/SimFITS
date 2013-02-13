#ifndef NOPATHLOSSMODEL_H_
#define NOPATHLOSSMODEL_H_

#include "SimplePathlossModel.h"


class NoPathlossModel : public SimplePathlossModel
{

public:

	virtual void filterSignal(Signal& s);
	virtual double calcPathloss(const Coord& myPos, const Coord& sendersPos);
};

#endif /*NOPATHLOSSMODEL_H_*/
