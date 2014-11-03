#include "fuellevel.h"
#include <QPainter>
#include <qmath.h>
#include <qtimer.h>
#include "InfoMsg.h"

FuelLevel::FuelLevel(const char* keyname ,int type, QWidget *parent)
	:BaseMeter(keyname,type, parent)
{
	m_componment =0x0f0e ;

	m_value = 0;
	m_minValue = 0;
	m_maxValue = 100;

	m_lowervalue = 25;
	m_lwarningvalue = 25;	
	m_hwarningvalue = 100;
	m_uppervalue = 100;

	m_startAngle = 120;
	m_endAngle = 120;

	m_scaleMajor = 4;
	m_scaleMiddle = 2;
	m_scaleMinor = 4;
	m_title = QStringLiteral("油位");

	QTimer *timer = new QTimer(this);
	timer->connect(timer,SIGNAL(timeout()),this,SLOT(changeValue()));
	timer->start(200);
}

void FuelLevel::paintEvent(QPaintEvent *e)
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
	
   	drawScale(&painter,center,radius());				/* 画刻度线 */

	qreal y = radius() * qSin(m_startAngle);			/* 绘制文字*/
	y= m_startAngle < 90?y:-y;
	drawText(&painter,QRect(-radius()+5,y+15,20,15),"E");
	drawText(&painter,QRect(radius()-25,y+15,20,15),"F");

	if(m_componment & Title)							/*画表盘名称*/
	{
		QRect r(-radius()/2,radius()-40,radius(),20);
		drawTitle(&painter,r);
	}
	drawlogo(&painter);									/* 画油位logo */
    drawIndicator(&painter);							/* 画指针 */
	
}

void FuelLevel::drawlogo(QPainter *painter)
{
	painter->save();
	painter->setPen(foreground());
	if(value()>=0.2*maxValue())
		painter->setBrush(Qt::blue);
	else
		painter->setBrush(Qt::red);

	painter->setPen(QPen(Qt::white,1));
	QPainterPath path;
	double x = radius()/2-20;
	double y = -radius()/2+40;
	path.moveTo(x,y);
	path.lineTo(x,y-40);
	path.lineTo(x+20,y-40);
	path.lineTo(x+20,y);
	painter->drawPath(path);
	painter->drawLine(x-5,y,x+25,y);
	
	painter->drawLine(x,y-20,x+20,y-20);
	painter->drawArc(x+15,y-20,10,10,0,90*16);
	painter->drawLine(x+25,y-15,x+25,y-8);
	painter->drawArc(x+25,y-11,6,6,180*16,180*16);
	painter->drawLine(x+31,y-8,x+31,y-20);
	painter->drawArc(x+25,y-26,6,12,0,90*16);
	painter->restore();
}
void FuelLevel::changeValue()
{
	//InfoMsg info;
	static double offset = 1.0;
	double val = value();
	if((val <= m_minValue && offset < 0) ||(val >= m_maxValue && offset >0))
	{
		offset = -offset;
	}
	setValue(val+offset);
}