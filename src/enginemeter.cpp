#include "enginemeter.h"
#include <QPainter>
#include <qmath.h>
#include <qtimer.h>
#include "InfoMsg.h"
#include <qmessagebox.h>
EngineMeter::EngineMeter(const char* keyname ,int type,QWidget *parent )
	:BaseMeter(keyname,type,parent)
{
	m_runtime = 0;
	m_value = 0;
    m_minValue = 0;
    m_maxValue = 10;
	m_lowervalue = 0;
	m_uppervalue = 6;
	m_lwarningvalue = 0;
	m_hwarningvalue = 6;

	m_startAngle =60;
	m_endAngle =60;

	m_scaleMajor = 10;
	m_scaleMiddle = 2;
    m_scaleMinor = 4;

	/*
	m_scaleMajorLength = 8;
	m_scaleMiddleLength = 4;
	m_scaleMinorLength = 2;
	*/

	m_units = QStringLiteral("RPM"); 
	m_title = QStringLiteral("转速");
	m_componment =0x0C0E;

	QTimer *timer = new QTimer(this);
	timer->connect(timer,SIGNAL(timeout()),this,SLOT(changeValue()));
	timer->connect(timer,SIGNAL(timeout()),this,SLOT(setRuntime()));
	timer->start(1000);
}

void EngineMeter::paintEvent(QPaintEvent *e)
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

	if(m_componment & NumericValue)
    {   
		drawNumericValue(&painter);						/* 画数字显示 */
	}

    drawIndicator(&painter);							/* 画指针 */
	drawRuntime(&painter);								/* 运行时间显示 */
	
}

void EngineMeter::drawRuntime(QPainter *painter)
{
	painter->save();
	int number[4];
	int t = m_runtime/60;
	int N=1000;
	for(int i=0;i<4;i++)
	{
		number[i] = t/N;
		t%=N;
		N/=10;
	}
	QRect r[4];
	for(int i=0;i<4;i++)
		r[i].setRect(-40+i*20,20,16,20);
	painter->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
	for(int i=0;i<4;i++)
		painter->drawRect(r[i]);
	painter->setPen(QPen(Qt::white,16));
	for(int i=0;i<4;i++)
		painter->drawText(r[i],Qt::AlignCenter,QString("%1").arg(number[i]));
	painter->restore();

	drawText(painter,QRect(40,20,10,20),"h");
}
void EngineMeter::setRuntime()
{
	m_runtime++ ;    //sec
	update();
}
void EngineMeter::changeValue()
{
	static double offset = 0.1;
	double val = value();
	if((val<=m_minValue && offset < 0) ||(val>=m_maxValue && offset >0))
		offset = -offset;
	setValue(val+offset);
}