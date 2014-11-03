#include "basemeter.h"
#include <QPainter>
#include <qmath.h>
#include <qtimer.h>
#include "InfoMsg.h"
#include <qmessagebox.h>

BaseMeter::BaseMeter(const char* keyname ,int type,QWidget *parent )
	:BaseParam(parent,keyname,type)
{
    m_value = 0;
    m_minValue = 0;
    m_maxValue = 100;
	m_lowervalue = 10;
	m_uppervalue = 90;
	m_lwarningvalue = 20;
	m_hwarningvalue = 80;

	m_scale = 1;
	m_strech = 30;
	m_innerScaleSteps = 20;

    m_precision = 0;
    m_units = QStringLiteral("N/A");
	m_title = QStringLiteral("名称");
    
	m_scaleMajor = 10;
	m_scaleMiddle = 5;
    m_scaleMinor = 10;
	m_scaleMajorLength = 8;
	m_scaleMiddleLength = 4;
	m_scaleMinorLength = 2;
	
    m_startAngle = 60;
    m_endAngle = 60;
	m_radius = 90;

    m_crownColor = QColor(160,160,160);
    m_foreground = QColor(255,255,255);
    m_background = QColor(65,65,65);
   
	m_componment = 0x000e; /*black backgroud,warning*/
#if TEST
	shift = 1;
#endif
	QTimer *timer = new QTimer(this);
	timer->connect(timer,SIGNAL(timeout()),this,SLOT(changeValue()));
	timer->start(500);
}



void BaseMeter::paintEvent(QPaintEvent *)
{
	QPointF center(0,0);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 		/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);		/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);			/* 比例缩放 */
    painter.setPen(Qt::NoPen);

	if(m_componment & Crown)
		drawCrown(&painter);							/* 画边框 */

	if(m_componment & BackGround)
		drawBackground(&painter);						/* 画背景 */							
	
   	drawScale(&painter,center,m_radius);				/* 画刻度线 */
	drawScaleNum(&painter,center,m_radius);				/* 画刻度值 */

	if(m_componment & Units)							/* 画单位 */
	{
		QRect r(-20,-m_radius/2,40,20);
		drawText(&painter,r,m_units);
	}

	if(m_componment & Title)							/*画表盘名称*/
	{
		QRect r(-m_radius/2,m_radius-40,m_radius,20);
		drawTitle(&painter,r);
	}

	if(m_componment & DoubleMeter)
	{
		drawInnerScale(&painter,center,m_radius-m_strech);		/*画双表盘*/
		drawInnerScaleNum(&painter,center,m_radius-m_strech);
	}

	if(m_componment & NumericValue)
    {   
		drawNumericValue(&painter);						/* 画数字显示 */
	}

    drawIndicator(&painter);								/* 画指针 */
}

void BaseMeter::drawCrown(QPainter *painter)
{
    painter->save();
    int radius = 100;
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter->setBrush(lg1);
    painter->drawEllipse(-radius, -radius, radius << 1, radius << 1);
    painter->restore();
}

void BaseMeter::drawBackground(QPainter *painter)
{
	// 画背景
	painter->save();
	painter->setBrush(Qt::black);
	painter->drawRect(-width(),-height(),2*width(),2*height());
	painter->restore();
	
	//画边框
	/*
	painter->save();
	painter->setBrush(m_background);
    painter->drawEllipse(-92, -92, 184, 184);
    painter->restore();
	*/
}

void BaseMeter::drawScale(QPainter *painter,QPointF center,double radius)
{
    painter->save();
    painter->rotate(m_startAngle);
    int steps = (m_scaleMajor * m_scaleMinor);
    double angleStep = (360.0 - m_startAngle - m_endAngle) / steps;
    painter->setPen(m_foreground);
	int scalerange = m_maxValue - m_minValue;
	int lowerwarning = steps * m_lwarningvalue / scalerange ;
	int upperwarning = steps * m_hwarningvalue / scalerange;
	int loweralarm = steps * m_lowervalue / scalerange;
	int upperalarm	= steps * m_uppervalue / scalerange;

    QPen pen = painter->pen();
	QColor color = pen.color();
    for (int i = 0; i <= steps; i++)
    {
		if(m_componment & 0x0f00 )
		{
			if(((m_componment&0x0100) && (i <= loweralarm)) || ((m_componment&0x0800) && (i>= upperalarm)) )
				color = QColor(Qt::red);
			else if(((m_componment&0x0200) &&( i <= lowerwarning)) || ((m_componment&0x0400) && (i>= upperwarning)))
				color = QColor(Qt::yellow);
			else
				color = QColor(Qt::white);
		}
		else
		{
			if(i%m_scaleMinor == 0 || i%m_scaleMiddle == 0)
				color = QColor(Qt::darkCyan);
			else
				color = QColor(Qt::white);
		}

		pen.setColor(color);
		if (i % m_scaleMinor == 0)
        {
            pen.setWidth(4);
            painter->setPen(pen);  
			painter->drawLine(center.x(), radius, center.y(), radius-m_scaleMajorLength);	
        }
		
		else if(i%m_scaleMiddle == 0)
		{
			pen.setWidth(2);
            painter->setPen(pen);
			painter->drawLine(0, radius, 0, radius-m_scaleMiddleLength);
		}
		
        else
        {
            pen.setWidth(2);
            painter->setPen(pen);
			painter->drawLine(0, radius, 0, radius-m_scaleMinorLength);
		}
        painter->rotate(angleStep);
    }
    painter->restore();
}

void BaseMeter::drawScaleNum(QPainter *painter,QPointF center,double  radius)
{
	//InfoMsg info;
    painter->save();
    painter->setPen(m_foreground);
    double startRad = (360 - m_startAngle - 90) * (3.14 / 180);
	double steps = (m_maxValue - m_minValue)/m_scaleMajor;
	double deltaRad = (360 - m_startAngle - m_endAngle) * (3.14 / 180) / m_scaleMajor;
    double sina,cosa;
    double x, y;
	
    QFontMetricsF fm(this->font());
  
	double w, h, tmpVal;
    QString str;

    for (int i = 0; i <= m_scaleMajor; i++)
    {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);

        tmpVal = i * steps + m_minValue;
        str = QString("%1").arg(tmpVal);
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = center.x()+(radius-18) * cosa - w / 2;
        y = center.y()-(radius-18) * sina - h / 2;
		const QRectF r(x,y,w,h);
		QFont font;
		font.setPixelSize(14);
		painter->setFont(font);
		painter->setPen(QPen(Qt::green));
		painter->drawText(r,Qt::AlignCenter,str);
	}
    painter->restore();
}


void BaseMeter::drawInnerScale(QPainter *painter, QPointF center,double radius)
{
	painter->save();
    painter->rotate(m_startAngle);
	double c_x = center.x();
	double c_y = center.y();
	int steps = (m_maxValue - m_minValue)*m_scale/m_innerScaleSteps *2;
	double angleStep = (360.0 - m_startAngle - m_endAngle) / steps;
    painter->setPen(m_foreground);
    QPen pen = painter->pen();
	pen.setColor(Qt::white);
    for (int i = 0; i <= steps; i++)
    {
        if (i % 2 == 0) //draw major tick
        {
            pen.setWidth(2);
            painter->setPen(pen);  
			painter->drawLine(c_x,radius, c_y, radius-6);	
        }
		else
        {
            pen.setWidth(2);
            painter->setPen(pen);
			painter->drawLine(c_x, radius, c_y, radius-3);
		}
        painter->rotate(angleStep);
    }
    painter->restore();
}
void BaseMeter::drawInnerScaleNum(QPainter *painter,QPointF center,double radius)
{
	painter->save();
    painter->setPen(m_foreground);

    double startRad = (360 - m_startAngle - 90) * (3.14 / 180);
	
	int steps = (m_maxValue - m_minValue)*m_scale/m_innerScaleSteps;
	double deltaRad = (3.14/180) * (360.0 - m_startAngle - m_endAngle) / steps;
    double sina,cosa;
    int x, y;
	
    QFontMetricsF fm(this->font());
	double w, h, tmpVal;
    QString str;
	painter->setPen(QPen(QColor(120,240,240),2,Qt::SolidLine,Qt::SquareCap));
    for (int i = 0; i <= steps; i++)
    {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);

		tmpVal = i *m_innerScaleSteps +m_minValue*m_scale;
        str = QString( "%1" ).arg(tmpVal);
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = (radius-15) * cosa - w / 2;
        y = -(radius-15) * sina - h / 2;
		const QRectF r(x,y,w,h);
		QFont font;
		font.setPixelSize(8);
		painter->setFont(font);
		painter->drawText(r,Qt::AlignCenter,str);
	}

	QRect rect(-radius-1,-radius-1,2*radius+2,2*radius+2);
	painter->drawArc(rect,(m_endAngle-90)*16,(360-m_startAngle-m_endAngle)*16);
    painter->restore();
}

/*title*/
void BaseMeter::drawTitle(QPainter *painter,QRect rect)
{
    painter->save();
    painter->setPen(m_foreground);
    painter->setBrush(m_foreground);
    QFontMetricsF fm(this->font());
	QFont font;
	font.setPixelSize(20);
	painter->setFont(font);
	painter->drawText(rect,Qt::AlignCenter,m_title);
    painter->restore();
}

void BaseMeter::drawText(QPainter *painter,QRect rect,const QString str)
{
    painter->save();
    painter->setPen(m_foreground);
    painter->setBrush(m_foreground);
    QFontMetricsF fm(this->font());
	QFont font;
	font.setPixelSize(20);
	painter->setFont(font);
	painter->drawText(rect,Qt::AlignCenter,str);
    painter->restore();
}

void BaseMeter::drawContexts(QPainter *painter,const QString str)
{
	painter->save();
    painter->setPen(m_foreground);
    painter->setBrush(m_foreground);
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
	double h = fm.size(Qt::TextSingleLine,str).height();
	QFont font;
	font.setPixelSize(10);
	QRect rect(-m_radius,0,w,h);
	painter->setFont(font);
	painter->drawText(rect,Qt::AlignVCenter|Qt::AlignLeft, str);
	painter->restore();
}

void BaseMeter::drawNumericValue(QPainter *painter)
{
	QString str  =  QString("%1").arg(m_value*m_scale, 0, 'f', m_precision);
    QFontMetricsF fm(font());
    double w = fm.size(Qt::TextSingleLine,str).width();
	double h = fm.size(Qt::TextSingleLine,str).height();
    painter->setPen(m_foreground);
	QRect rect(-w/2,10,w,h);
	painter->drawText(rect,Qt::AlignCenter, str);
}

void BaseMeter::drawIndicator(QPainter *painter)
{
    painter->save();
    QPolygon pts;
	int py = radius()-m_scaleMajorLength-2;
	pts.setPoints(3, -2,0, 2,0, 0,py);	/* (-2,0)/(2,0)/(0,60) */

	//draw needle
	double degRotate = (360.0 - m_startAngle - m_endAngle)/(m_maxValue - m_minValue)*(m_value - m_minValue);
	painter->rotate(degRotate+double(m_startAngle));
    QRadialGradient haloGradient(0, 0, 80, 0, 0);
    haloGradient.setColorAt(0, QColor(255,255,10));
    haloGradient.setColorAt(1, QColor(255,255,0));
    painter->setPen(QColor(255,255,0));
    painter->setBrush(haloGradient);
    painter->drawConvexPolygon(pts);
    painter->restore();

    //画中心点
    QColor niceBlue(150, 150, 200);
    QConicalGradient coneGradient(0, 0, -90.0);
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);
    painter->setPen(Qt::NoPen);
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);
}

void BaseMeter::setComponment(int componment,bool enabled )
{
	if(enabled)
		m_componment |= componment;
	else
		m_componment &= ~componment;
	update();
}

void BaseMeter::setValue(double value)
{
    if(value > m_maxValue)
		m_value = m_maxValue;
    else if(value < m_minValue)
		m_value = m_minValue;
    else
		m_value = value;
    update();
}

void BaseMeter::setMinValue(int value)
{
    if (value < m_maxValue)
    {
        m_minValue = value;
        update();
    }
}

void BaseMeter::setMaxValue(int value)
{
    if(value > m_minValue)
    {
        m_maxValue = value;
        update();
    }
}

void BaseMeter::setLowerValue(int val)
{
	m_lowervalue = val;
	update();
}
void BaseMeter::setUpperValue(int val)
{
	m_uppervalue = val;
	update();
}
void BaseMeter::setLowerWarningValue(int val)
{
	m_lwarningvalue = val;
	update();
}
void BaseMeter::setUpperWarningValue(int val)
{
	m_hwarningvalue = val;
	update();
}

void BaseMeter::setScale(double scale)
{
	m_scale = scale;
	update();
}
void BaseMeter::setStrech(int strech)
{
	m_strech = strech;
	update();
}
void BaseMeter::setInnerScaleSteps(int steps)
{
	m_innerScaleSteps = steps;
	update();
}
void BaseMeter::setPrecision(int precision)
{
    if (precision <= 3)
    {
        m_precision = precision;
        update();
    }
}

void BaseMeter::setUnits(QString units)
{
    m_units = units;
    update();
}

void BaseMeter::setTitle(QString title)
{
	m_title = title;
    update();
}

void BaseMeter::setScaleMajor(int scale)
{
    m_scaleMajor = scale;
    update();
}

void BaseMeter::setScaleMiddle(int scale)
{
    m_scaleMiddle = scale;
    update();
}
void BaseMeter::setScaleMinor(int scale)
{
    m_scaleMinor = scale;
    update();
}

void BaseMeter::setScaleMinorLength(int scalelen)
{
    m_scaleMinorLength = scalelen;
    update();
}

void BaseMeter::setScaleMiddleLength(int scalelen)
{
    m_scaleMiddleLength = scalelen;
    update();
}

void BaseMeter::setScaleMajorLength(int scalelen)
{
    m_scaleMajorLength = scalelen;
    update();
}




void BaseMeter::setStartAngle(int value)
{
    m_startAngle = value;
    update();
}

void BaseMeter::setEndAngle(int value)
{
    m_endAngle = value;
    update();
}
void BaseMeter::setRadius(double radius)
{
	m_radius = radius;
	update();
}
void BaseMeter::setCrownColor(QColor newColor)
{
    m_crownColor = newColor;
    update();
}

void BaseMeter::setForeground(QColor newForeColor)
{
    m_foreground = newForeColor;
    update();
}

void BaseMeter::setBackground(QColor newBackColor)
{
    m_background = newBackColor;
    update();
}

void BaseMeter::setWork(bool work)
{
	m_work = work;
	setVisible(work);
}
void BaseMeter::changeValue()
{
	double val = value();
	if((val <= m_minValue && shift < 0) || (val >= m_maxValue && shift > 0))
		shift = -shift;
	setValue(val+shift);
}
