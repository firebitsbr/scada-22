#ifndef FUELLEVEL_H
#define FUELLEVEL_H

#include "basemeter.h"

class FuelLevel : public BaseMeter
{
	Q_OBJECT
public:
	explicit FuelLevel(	const char* keyname = "fuellevel",int type = Type::Meter, QWidget *parent = 0);
	 
	~FuelLevel() {}
	void drawlogo(QPainter *);
private:
	void paintEvent(QPaintEvent *);

private Q_SLOTS:
	void changeValue();
};

#endif //FUELLEVEL_H
