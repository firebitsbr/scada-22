#include "basepage.h"
#include "qlayout.h"
#include "qpalette.h"
#include "qframe.h"
#include "InfoMsg.h"
#include <qmessagebox.h>

BasePage::BasePage(QString name,int row,int column,int num,QWidget *parent)
	: QWidget(parent),m_pageName(name),m_row(row),m_column(column),m_MeterCounts(num)
{
	QPalette p = palette();
	p.setBrush(QPalette::Background,Qt::black);
	setPalette(p);
	setAutoFillBackground(true);
	QFrame(this,Qt::FramelessWindowHint);
	
	m_gridlayout = new QGridLayout;
	m_lamplayout = new QHBoxLayout;
	//m_mainlayout = new QVBoxLayout;
	LampEnabled = false;			/*Not have lamps default */
	m_LampCounts = 0;
	m_MeterCounts = 0;
	m_counts =0;
	setLayout(m_gridlayout);
	setWindowTitle(m_pageName);
	connect(this,SIGNAL(lampNumChange()),this,SLOT(lampNumChangeEvent()));
	connect(this,SIGNAL(meterNumChange()),this,SLOT(meterNumChangeEvent()));
}

BasePage::~BasePage()
{

}


//void BasePage::resizeEvent(QResizeEvent *)
//{
//	//QMessageBox::about(0,"size",QString("%1,%2").arg(width()).arg(height()));
//	//if(m_LampCounts>0)
//	//{
//	//	m_gridlayout->setGeometry(QRect(0,0,width(),height()*0.75));
//	//	m_lamplayout->setGeometry(QRect(0,height()*0.75,width(),height()*0.25));
//	//}
//}


void BasePage::insertMeter(QWidget *w , int row, int column)
{
	if(m_MeterCounts> m_row * m_column)
	{
		QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("此页面不能再继续插入页面！"));
		return;
	}
	row = qMin(m_row,row);
	column = qMin(m_column,column);
	int pos = (row-1) * m_column + column-1;
	pos = pos>m_MeterCounts?m_MeterCounts:pos;
	appendMeter((BaseParam*)w,pos);
}

void BasePage::appendMeter(BaseParam *w , int pos)
{
	
	int type = w->getType();
	if(type <= 0 || type > 3)
	{
		QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("添加控件类型错误，只能添加表盘，温度计和指示灯！"));
		return;
	}
	if(pos<0)
	{
		QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("请选择正确的添加仪表的位置！"));
		return;
	}
	//QMessageBox::about(this,"counts",QString("meter:%1,lamp%2,counts%3,pos:%4").arg(m_MeterCounts).arg(m_LampCounts).arg(m_counts).arg(pos));
	if(type == 1 || type == 2)
	{
		if(m_MeterCounts >= m_row * m_column)
		{
			QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("此页面不能再继续插入仪表！(%1*%2)").arg(m_MeterCounts).arg(m_row*m_column));
			return;
		}
		pos = pos>m_MeterCounts?m_MeterCounts:pos-1;
		//QMessageBox::about(this,"Meter",QString("meter:%1,lamp%2,counts%3,pos:%4").arg(m_MeterCounts).arg(m_LampCounts).arg(m_counts).arg(pos));
		m_vectMeter.insert(pos,w);
		emit meterNumChange();
	}
	else
	{
		//QMessageBox::about(0,"num",QString("meter:%1,m_lamp:%2,m_counts:%3,pos:%4").arg(m_MeterCounts).arg(m_LampCounts).arg(m_counts).arg(pos));
		if(pos<=m_MeterCounts)
			pos = m_MeterCounts;
		else
			pos = pos > m_counts?m_counts:pos-1;
		LampEnabled = true;
		//QMessageBox::about(0,"Lamp",QString("meter:%1,m_lamp:%2,m_counts:%3,pos:%4").arg(m_MeterCounts).arg(m_LampCounts).arg(m_counts).arg(pos));
		m_vectMeter.insert(pos,w);
		emit lampNumChange();
	}
}

void BasePage::deleteWidget(QWidget *w , int row, int column)
{
	row = row > m_row? m_row: row;
	column = column > m_column?m_column: column;
	QVector<BaseParam *>::iterator  iter =  m_vectMeter.begin() +(row-1) * m_column + column-1;
	int type = (*iter)->getType();
	if(type == 1 || type == 2)
	{
		m_MeterCounts--;
		m_counts--;
	}
	else
	{
		m_LampCounts--;
		m_counts--;
	}
	m_vectMeter.erase(iter);
	updateLayout();
}

BaseParam* BasePage::GetMeter(const int i) const
{
	return m_vectMeter[i];
}
/**/
void BasePage::updateLayout()
{
	for(int i =0 ; i<m_MeterCounts ;i++)
	{
		m_gridlayout->addWidget(m_vectMeter[i],i/m_column,i%m_column);
	}
	if(LampEnabled)
	{
		for(int i = 0;i<m_LampCounts;i++)
			m_lamplayout->addWidget(m_vectMeter[m_MeterCounts + i]);
		m_gridlayout->addLayout(m_lamplayout,m_row,0,1,m_column);
	}
	m_gridlayout->update();
//	m_mainlayout->update();
}

void BasePage::setRow(int row)
{
	m_row = row;
	updateLayout();
}

void BasePage::setColumn(int column)
{
	m_column = column;
	updateLayout();
}
void BasePage::lampNumChangeEvent()
{
	m_LampCounts++;
	m_counts++;
	updateLayout();
}
void BasePage::meterNumChangeEvent()
{
	m_MeterCounts++;
	m_counts++;
	updateLayout();
}
