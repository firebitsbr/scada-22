#ifndef BASEMETER_H
#define BASEMETER_H

#include <QWidget>
#include "baseparam.h"

#define TEST 1

class BaseMeter : public BaseParam
{
	Q_OBJECT
public:     
	enum Componment{
		NoComponment = 0x0,
		Crown		= 0x01,			/*draw crown*/
		BackGround	= 0x02,			/*draw background*/
		Units		= 0x04,			/*draw units*/
		Title		= 0x08,			/*draw title*/
		Logo		= 0x10,			/*draw log*/
		DoubleMeter = 0x20,			/*draw inner meter*/
		NumericValue= 0x40,			/*show numeric value*/

		Lower		 = 0x0100,      /* lower */
		LowerWarning = 0x0200,
		UpperWarning = 0x0400,		/*upper*/
		Upper		 = 0x0800
	};
	explicit BaseMeter(const char* keyname ,int type = Type::Meter,QWidget *parent =0);
	virtual ~BaseMeter() {  }

	double radius() const {return m_radius;}
    double value() const {return m_value;}
    int minValue() const {return m_minValue;}
    int maxValue() const {return m_maxValue;}
	int LowerValue() const {return m_lowervalue;}
	int UpperValue() const {return m_uppervalue;}
	int LowerWarningValue() const {return m_lwarningvalue;}
	int UpperWarningValue() const {return m_hwarningvalue;}
    int precision() const {return m_precision;}
	int Strech() const {return m_strech;}
    QString units()const {return m_units;}
    QString title() const {return m_title;}
    int scaleMajor() const {return m_scaleMajor;}
	int scaleMiddle() const {return m_scaleMiddle;}
	int scaleMinor() const {return m_scaleMinor;}
	int scaleMajorLength() const {return m_scaleMajorLength;}
	int scaleMiddleLength() const {return m_scaleMiddleLength;}
    int scaleMinorLength() const {return m_scaleMinorLength;}
    int startAngle() const {return m_startAngle;}
    int endAngle() const { return m_endAngle;}
	
    QColor crownColor() const {return m_crownColor;}
    QColor foreground() const {return m_foreground;}
    QColor background() const {return m_background;}
	int getComponents() const {return m_componment;}
	bool isWork() const {return m_work;}

	
	virtual void drawCrown(QPainter *painter);
    virtual void drawBackground(QPainter *painter);
	virtual void drawScale(QPainter *painter,QPointF center,double radius);
	virtual void drawScaleNum(QPainter *painter,QPointF center,double radius);
    virtual void drawIndicator(QPainter *painter);
    virtual void drawNumericValue(QPainter *painter);
	virtual void drawInnerScale(QPainter *painter,QPointF center,double radius);
	virtual void drawInnerScaleNum(QPainter *painter,QPointF center,double radius);
	virtual void drawTitle(QPainter *painter,QRect r);
	virtual void drawText(QPainter *painter,QRect r,const QString str);
	virtual void drawContexts(QPainter *painter, const QString str);
#if TEST
	void setShift(double val) { shift = val;}
#endif
public Q_SLOTS:
    void setMinValue(int);
    void setMaxValue(int);
    
	void setLowerValue(int);
	void setUpperValue(int);
	void setLowerWarningValue(int);
	void setUpperWarningValue(int);
	void setScale(double);
	void setStrech(int);

	void setPrecision(int);
    void setUnits(QString);
    void setTitle(QString);

    void setScaleMajor(int);
	void setScaleMiddle(int);
    void setScaleMinor(int);
	void setScaleMajorLength(int);
	void setScaleMiddleLength(int);
	void setScaleMinorLength(int);
	void setInnerScaleSteps(int);

    void setStartAngle(int);
    void setEndAngle(int);
	void setRadius(double);

    void setCrownColor(QColor);
    void setForeground(QColor);
    void setBackground(QColor);

	void setValue(double);
	void setWork(bool);
	void setComponment(int ,bool enabled = true); 
	virtual void changeValue();

protected:
	
	QString m_units;
	QString  m_title;
	int m_componment;
    double m_value;
    int m_maxValue, m_minValue;
	int m_lwarningvalue,m_hwarningvalue;
	int m_lowervalue,m_uppervalue;
    int m_precision;

	double m_scale;
	int m_strech;

    int m_scaleMajor;
	int m_scaleMiddle;
    int m_scaleMinor;
	int m_scaleMajorLength;
	int m_scaleMiddleLength;
	int m_scaleMinorLength;

	int m_innerScaleSteps;
    int m_startAngle,m_endAngle;
	double m_radius;
    QColor m_crownColor;
    QColor m_foreground;
    QColor m_background;
	bool m_work;

	virtual void paintEvent(QPaintEvent *);
/*test parameter*/	
#if TEST
private:
	double shift;
#endif
};

#endif // BASEMETER_H
