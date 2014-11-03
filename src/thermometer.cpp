#include "thermometer.h"
#include <QPainter>
#include "InfoMsg.h"

Thermometer::Thermometer(const char* keyname ,int type,QWidget *parent )
	:BaseParam(parent,keyname,type)
{
	m_value = 0;
	m_minValue = -30;
	m_maxValue = 70;
	m_thresholdUp = -20;
	m_thresholdDown = 50;
	m_thresholdFlag = false;
	m_units = QStringLiteral("℃");
	m_title = QStringLiteral("温度");
	m_steps = 10;
	m_scaleMajor = 10;
	m_scaleMinor = 5;
	m_borderColor = Qt::blue;
	m_foreground = Qt::white;
	m_background = QColor(41, 137, 254);
	m_liquidColor = Qt::green;
/*set the background color*/
	QPalette p = palette();
	p.setBrush(QPalette::Background,Qt::black);
	setPalette(p);
	setAutoFillBackground(true);
}

void Thermometer::paintEvent(QPaintEvent *)
{
	//InfoMsg info;
	//QRect rect = geometry();
	//info.info_log("Thermometer:,%d,%d,%d,%d,%d,%d\n",rect.x(),rect.y(),rect.width(),rect.height(),width(),height());
	int w = width();
	int h = height();
	//if ( qAbs(3*w -  h) <= 0.0001)
	if ( 3*w < h)
		h = 3 * w;
	else
		w = h/3;
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
	painter.translate(width() / 2, height() / 2 );	/* 坐标变换为窗体中心 */
	int side = qMin(width(), height());
	painter.scale(w / 200.0, w / 200.0);		/* 比例缩放 */
	//rect = geometry();
	//info.info_log("Thermometer2:,%d,%d,%d,%d\n",rect.x(),rect.y(),rect.width(),rect.height());
	painter.setPen(Qt::NoPen);
	m_scaleMajor = (m_maxValue - m_minValue)/m_steps;
	int nSteps = m_scaleMajor * m_scaleMinor;
	double step = 400.0 / nSteps;
	QFont font = this->font();

	/* 绘制边框 */
	/*
	painter.save();
	painter.setPen(QPen(m_borderColor, 4));
	painter.drawRect(-100 , -250 , 200 , 550 );
	painter.restore();
	*/

	/* 绘制背景 */
	/*
	painter.save();
	painter.setBrush(m_background);
	painter.drawRect(-90 , -240 , 180 , 480 );
	painter.restore();
	*/

	/* 绘制液体容器圆 */
	painter.save();
	painter.setBrush(m_foreground);
	painter.setBrush(Qt::gray);
	painter.drawEllipse(-50 , 230 , 40 , -40 );
	painter.restore();
		
	/* 绘制液体容器柱 */
	painter.save();
	//painter.setBrush(m_foreground);
	painter.setBrush(Qt::gray);
	painter.drawRect(-40 , 210 , 20 , -440 );
	painter.restore();
	
	/* 绘制单位 */
	painter.save();
	painter.setPen(m_foreground);
	font.setPointSize(18);
	font.setBold(true);

	painter.setFont(font);
	painter.setPen(QPen(Qt::yellow));
	painter.drawText(QRect(15 , 190 ,50  ,50 ),Qt::AlignCenter,m_units);
	painter.restore();

	/* 绘制刻度线 */
	painter.save();
	QPen pen(m_foreground);
	for (int i = 0, y = 180.0; i <= nSteps; i++, y -= step)
	{
		if (i % m_scaleMinor == 0)
		{
			pen.setWidth(3);
			pen.setColor(Qt::green);
			painter.setPen(pen);
			painter.drawLine(-20 , y , 0, y );
		}
		else
		{
			pen.setWidth(1);
			pen.setColor(Qt::yellow);
			painter.setPen(pen);
			painter.drawLine(-20 , y , -10 , y );
		}
	}
	painter.restore();
	
	/* 绘制刻度值 */
	painter.save();
	painter.setPen(m_foreground);
	double tmpVal;
	step = 400.0 / m_scaleMajor;
	for (int i = 0, y = 185.0; i <= m_scaleMajor; i++, y -= step)
	{
		tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;
		font.setPixelSize(22);
		//painter.setPen(QPen(Qt::green));
		painter.setFont(font);
		painter.drawText(QRect(10 ,(y-20) ,40 ,30 ),Qt::AlignCenter, QString( "%1" ).arg(tmpVal));
	}
	painter.restore();
	
	/* 绘制液体圆 */
	painter.save();
	painter.setBrush(m_liquidColor);
	painter.drawEllipse(-45 , 225 , 30 , -30 );
	painter.restore();
	
	/* 绘制液体柱(指示) */
	painter.save();
	painter.setBrush(m_liquidColor);
	double val =  400.0 /(m_maxValue - m_minValue)*(m_value - m_minValue);
	painter.drawRect(-35 , 205 , 10 , (-val - 25) );
	painter.restore();

	painter.save();
	QRect r(-100,250,180,50);
	painter.setPen(Qt::white);
	font.setPixelSize(30);
	painter.setFont(font);
	painter.drawText(r,Qt::AlignCenter,m_title);
	painter.restore();
}

void Thermometer::setValue(double val)
{
	if (val > m_maxValue)
		m_value = m_maxValue;
	else if (val < m_minValue)
		m_value = m_minValue;
	else
		m_value = val;
		
	thresholdManager();
	update();
}

void Thermometer::setMinValue(int val)
{
	m_minValue = val;
	update();
}

void Thermometer::setMaxValue(int val)
{
	m_maxValue = val;
	update();
}

void Thermometer::setThresholdUp(double val)
{
	m_thresholdUp = val;
	thresholdManager();
	update();
}

void Thermometer::setThresholdDown(double val)
{
	m_thresholdDown = val;
	thresholdManager();
	update();
}
void Thermometer::setTitle(QString title)
{
	m_title = title;
	update();
}
void Thermometer::setUnits(QString str)
{
	m_units = str;
	update();
}

void Thermometer::setScaleMajor(int scale)
{
	m_scaleMajor = scale;
	update();
}

void Thermometer::setScaleMinor(int scale)
{
	m_scaleMinor = scale;
	update();
}
void Thermometer::setSteps(int steps)
{
	m_steps = steps;
	update();
}
void Thermometer::setBorderColor(QColor color)
{
	m_borderColor = color;
	update();
}

void Thermometer::setForeground(QColor color)
{
	m_foreground = color;
	update();
}

void Thermometer::setBackground(QColor color)
{
	m_background = color;
	update();
}

void Thermometer::setLiquidColor(QColor color)
{
	m_liquidColor = color;
	update();
}

void Thermometer::thresholdManager()
{
	if (!m_thresholdFlag)
	{
		if(m_value > m_thresholdUp)
		{
			m_thresholdFlag = true;
			Q_EMIT thresholdAlarm(AlarmUp);    
		}
		else if(m_value < m_thresholdDown)
		{
			m_thresholdFlag = true;
			Q_EMIT thresholdAlarm(AlarmDown);    
		}
			
	}
	else if (m_value <= m_thresholdUp && m_value >= m_thresholdDown)
	{
		m_thresholdFlag = false;
		Q_EMIT thresholdAlarm(AlarmNo);
	}
}

void Thermometer::setWork(bool work)
{
	m_work = work;
	setVisible(work);
}