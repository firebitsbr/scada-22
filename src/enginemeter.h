#ifndef ENGINEMETER_H
#define ENGINEMETER_H

#include "basemeter.h"

class EngineMeter : public BaseMeter
{
	Q_OBJECT
public:
	explicit EngineMeter(const char* keyname = "enginemeter",int type = Type::Meter, QWidget *parent =0);
	//explicit EngineMeter(const char* keyname ,int type = Type::Meter, QWidget *parent =0);
	~EngineMeter() {}
	void drawRuntime(QPainter *);
	int runtime() {return m_runtime;}
private:
	void paintEvent(QPaintEvent *);
	int m_runtime;

private Q_SLOTS:
	void setRuntime(); 
	void changeValue();
};

#endif //ENGINEMETER_H
