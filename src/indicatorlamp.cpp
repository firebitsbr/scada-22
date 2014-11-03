#include "indicatorlamp.h"
#include <QPainter>
#include <qtimer.h>
#include "InfoMsg.h"

IndicatorLamp::IndicatorLamp(const char *keyname, QString title, int type, QWidget *parent)
	:BaseParam(parent,keyname,type),m_title(title)
{
	m_alarm = false;
	m_work = true;
	
	/*set the background color*/
	QPalette p = palette();
	p.setBrush(QPalette::Background,Qt::black);
	setPalette(p);
	setAutoFillBackground(true);
#if 0
	QTimer *timer = new QTimer(this);
	timer->connect(timer,SIGNAL(timeout()),this,SLOT(changeWork()));
	timer->start(1000);
#endif

}

void IndicatorLamp::paintEvent(QPaintEvent *)
{
	int w = width(), h = height();
	if(w < h*2/3)
		h = w*3/2;
	else
		w = h*2/3;
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
	painter.translate(width()/2, height()/3);	/* 坐标变换为窗体中心 */
	painter.scale(w / 300.0, w / 300.0);		/* 比例缩放 */
	painter.setPen(Qt::NoPen);
	
    int radius = 100;

    /* 外边框 */
	painter.save();
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
	painter.restore();

    /* 内边框 */
	painter.save();
    radius -= 13;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
	painter.restore();

	/* 内部指示颜色 */
	painter.save();
	radius -= 4;
    QRadialGradient rg(0, 0, radius);
	if(!m_work)  //不工作时的状态
	{
		rg.setColorAt(0, QColor(230, 230, 230));
		rg.setColorAt(0.6, QColor(210, 210, 210));
		rg.setColorAt(1, QColor(190, 190, 190));
	}	
	else	 //工作时的状态
	{
		if (m_alarm)
		{	
			rg.setColorAt(0, QColor(245, 0, 0));
			rg.setColorAt(0.6, QColor(210, 0, 0));
			rg.setColorAt(1, QColor(166, 0, 0));
		}
		else
		{
			rg.setColorAt(0, QColor(0, 245, 0));
			rg.setColorAt(0.6, QColor(0, 210, 0));
			rg.setColorAt(1, QColor(0, 166, 0));
		}
	}
    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

	/*绘制文字*/
	QFontMetricsF fm(this->font());
	QFont font;
	font.setPixelSize(40);
	painter.setFont(font);
	painter.setPen(QColor(Qt::yellow));
	painter.drawText(QRect(-100,100,200,100),Qt::AlignCenter,m_title);
	painter.restore();
}

void IndicatorLamp::setAlarm(bool alarm)
{
	m_alarm = alarm;
	update();
}

void IndicatorLamp::setWork(bool work)
{
	m_work = work;
	//setVisible(work);
	update();
}

void IndicatorLamp::changeWork()
{
	static bool worked = false;
	worked = !worked;
	setWork(worked);
	setAlarm(worked);
}
