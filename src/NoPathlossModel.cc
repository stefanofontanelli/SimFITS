#include "NoPathlossModel.h"


void NoPathlossModel::filterSignal(Signal& s)
{
    return;
}

double NoPathlossModel::calcPathloss(const Coord& myPos, const Coord& sendersPos)
{
	return 1.0;
}
