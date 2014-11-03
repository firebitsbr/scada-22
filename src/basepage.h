

#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <qvector.h>
#include <qmap.h>
#include "baseparam.h"
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class BasePage : public QWidget
{
	Q_OBJECT

public:
	BasePage(QString name = QStringLiteral("ÐÂÒ³Ãæ"),int row = 1, int column = 3 ,int num = 0,QWidget *parent = 0);
	~BasePage();
	int MeterCounts() const {return m_MeterCounts;}
	int LampCounts() const {return m_LampCounts;}
	int Counts() const {return m_vectMeter.size();}
	void insertMeter(QWidget *w,int row, int col);
	void appendMeter(BaseParam *w,int pos );
	void deleteWidget(QWidget *w,int row,int col);

	BaseParam* GetMeter(const int i) const ;
	int Row() const {return m_row;}
	int Colomn() const {return m_column;}
	int MeterPos(QWidget *w) const ;
	QString pageName() const {return m_pageName;}

signals:
	void lampNumChange();
	void meterNumChange();

public Q_SLOTS:
	void setRow(int);
	void setColumn(int);
	void lampNumChangeEvent();
	void meterNumChangeEvent();
	//void setMeterCounts(int flag = 0){};
	void updateLayout();
	void setPageName(QString name) {m_pageName = name;}

private:
	QVector<BaseParam *> m_vectMeter;
	bool LampEnabled;
	int m_row;
	int m_column;
	int m_MeterCounts;
	int m_LampCounts;
	int m_counts;
	QString m_pageName;
	QGridLayout *m_gridlayout;
	QHBoxLayout *m_lamplayout;
	//QVBoxLayout *m_mainlayout;
	//void resizeEvent(QResizeEvent *);
};

#endif // BASEWIDGET_H
