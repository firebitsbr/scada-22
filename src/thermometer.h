#ifndef QCWTHERMOMETER_H
#define QCWTHERMOMETER_H

#include <QWidget>
#include "baseparam.h"

class Thermometer : public BaseParam
{
	Q_OBJECT

	//Q_ENUMS(errorCode);
	//Q_PROPERTY(double value READ value WRITE setValue);
	//Q_PROPERTY(int minValue READ minValue WRITE setMinValue);
	//Q_PROPERTY(int maxValue READ maxValue WRITE setMaxValue);
	//Q_PROPERTY(double thresholdUp READ thresholdUp WRITE setThresholdUp);
	//Q_PROPERTY(double thresholdDown READ thresholdDown WRITE setThresholdDown);
	//Q_PROPERTY(const char* units READ units WRITE setUnits);
 //   Q_PROPERTY(int scaleMajor READ scaleMajor WRITE setScaleMajor);
	//Q_PROPERTY(int scaleMinor READ scaleMinor WRITE setScaleMinor);
	//Q_PROPERTY(int steps READ steps WRITE setSteps);
	//Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor);
	//Q_PROPERTY(QColor foreground READ foreground WRITE setForeground);
	//Q_PROPERTY(QColor background READ background WRITE setBackground);
	//Q_PROPERTY(QColor liquidColor READ liquidColor WRITE setLiquidColor);
	//Q_PROPERTY(bool work READ isWork WRITE setWork);
	
public:
	enum AlarmCode {AlarmUp = 1, AlarmDown, AlarmNo};
	
	explicit Thermometer(const char* keyname ,int type = Type::Thermometer, QWidget *parent =0);
	double value() const {return m_value;}
    int minValue() const {return m_minValue;}
    int maxValue() const {return m_maxValue;}
    double thresholdUp() const {return m_thresholdUp;}
	double thresholdDown() const {return m_thresholdDown;}
	QString title() const {return m_title;}
	QString units() const {return m_units;}
	int scaleMajor() const {return m_scaleMajor;}
	int scaleMinor() const {return m_scaleMinor;}
	int steps() const {return m_steps;}
	QColor borderColor() const {return m_borderColor;}
    QColor foreground() const {return m_foreground;}
    QColor background() const {return m_background;}
	QColor liquidColor() const {return m_liquidColor;}
	bool isWork() const {return m_work;}
	//const char* getName() {return m_keyName;}
	//Type getType() {return m_type;}
	virtual void paintEvent(QPaintEvent *event);
Q_SIGNALS:
	void thresholdAlarm(enum AlarmCode);
	
public Q_SLOTS:
	void setValue(double);	
	void setMinValue(int);
	void setMaxValue(int);	
	void setThresholdUp(double);
	void setThresholdDown(double); 	
	void setUnits(QString);	
	void setTitle(QString);
	void setScaleMajor(int);
	void setScaleMinor(int);
	void setBorderColor(QColor);
	void setForeground(QColor);
	void setBackground(QColor);
	void setLiquidColor(QColor);
	void setSteps(int);
	void setWork(bool);
	//void setName(const char *name) { qstrncpy(m_keyName,name,50);}
	//void setType(Type type) {m_type = type;}
	

protected:
	void thresholdManager();
	
private:
	double m_value;
	int m_minValue;
	int m_maxValue;
	double m_thresholdUp;
	double m_thresholdDown;
	bool m_thresholdFlag;
	QString m_units;
	QString m_title;
	int m_scaleMajor;
	int m_scaleMinor;
	int m_steps;
	QColor m_borderColor;
	QColor m_foreground;
	QColor m_background;
	QColor m_liquidColor;
	bool m_work;
};

#endif
