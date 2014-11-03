#ifndef SCADA_H
#define SCADA_H

#include <QtWidgets/QWidget>
#include "ui_scada.h"

#include "basemeter.h"
#include "enginemeter.h"

class QPushButton;
class QStackedWidget;
class QConfigDlg;
class BasePage;

class Scada : public QWidget
{
	Q_OBJECT

public:
	Scada(QWidget *parent = 0);
	~Scada();

	
protected:
	void resizeEvent(QResizeEvent *);
	void closeEvent(QCloseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);

private:
	Ui::ScadaClass ui;

	QPushButton *m_prevPushButton;
	QPushButton *m_nextPushButton;
	QPushButton *m_exitPushButton;
	QPushButton *m_configPushButton;
	QPushButton *m_defaultPushButton;

	BasePage *m_mainpage;
	QStackedWidget *m_stackwidget;
	QConfigDlg *m_configDlg;
	static int m_currentPage;

	int m_maxpage;
	int getPageNum() {return m_maxpage;}
	void createUI();
	void initMainpage();

	void readSettings(QString filename = "default.ini");
	void writeSettings(QString filename = "default.ini");
		
	void  insertPage(int num = 1);
	void  insertMeterToPage(int pagenum, QWidget *w,int pos);
	void  test();

signals:
	void pageChange();

public slots:
	void setMaxPage() ;
	void rePage();
	void prevPushButton_clicked();
	void nextPushButton_clicked();
	void exitPushButton_clicked();
	void configDlgAccepted();
	void configDlgExec();
	void DefCfgPushButton_clicked();
};

#endif // SCADA_H
