#ifndef QCWINDCCATORLAMP_H
#define QCWINDCCATORLAMP_H

#include <QWidget>
#include "baseparam.h"

class QColor;

class IndicatorLamp: public BaseParam
{
	Q_OBJECT

	Q_PROPERTY(bool alarm READ isAlarm WRITE setAlarm);
	Q_PROPERTY(bool work READ isWork WRITE setWork);
	
public:    
	explicit IndicatorLamp(const char *keyname ,QString title = QStringLiteral("Ö¸Ê¾µÆ"), int type = Type::Lamp,QWidget *parent = 0);
	//IndicatorLamp(QWidget *parent = 0,QString m_title);
	bool isAlarm() const {return m_alarm;}
	bool isWork() const {return m_work;}
	QString title() const {return m_title;}
	virtual void paintEvent(QPaintEvent *);

public Q_SLOTS:
	void setAlarm(bool);
	void setWork(bool);
	void setTitle(QString title) { m_title = title;}
	void changeWork(); 
private:
	bool m_alarm;
	bool m_work;
	QString m_title;
};

#endif
