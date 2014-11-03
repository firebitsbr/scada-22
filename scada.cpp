#include "scada.h"
#include <qstackedwidget.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qstringlist.h>

#include "InfoMsg.h"
#include "qconfigdlg.h"
#include "basepage.h"
#include "basemeter.h"
#include "indicatorlamp.h"
#include "thermometer.h"
#include "fuellevel.h"
#include "enginemeter.h"
#include "configure.h"

#define DEFAULT 1

int Scada::m_currentPage = 0;
Scada::Scada(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QPalette p = palette();
	p.setBrush(QPalette::Background,Qt::black);
	setPalette(p);
	setAutoFillBackground(true);
	QFrame(this,Qt::FramelessWindowHint);

	m_stackwidget = new QStackedWidget(this);
	m_maxpage = 0;
	createUI();

#if 0
	initMainpage();
	test();
#else

#if DEFAULT
	readSettings("config.ini");
#else
	readSettings("default.ini");
#endif

#endif
}

Scada::~Scada()
{

}
void Scada::createUI()
{
	InfoMsg info;

	/*previous page button*/
	m_prevPushButton = new QPushButton(QString::fromLocal8Bit("上一页"),this);
	m_prevPushButton->setIcon(QIcon(QPixmap(":Resources/icon/prev.png")));
	m_prevPushButton->setIconSize(QSize(200,40));
	if(m_currentPage == 0 || m_stackwidget->count() <= 1)
		m_prevPushButton->setEnabled(false);
	/*next page button*/
	m_nextPushButton = new QPushButton(QString::fromLocal8Bit("下一页"),this);
	m_nextPushButton->setIcon(QIcon(":Resources/icon/next.png"));
	m_nextPushButton->setIconSize(QSize(200,40));
	if(m_stackwidget->count()<=1 || m_currentPage == m_maxpage-1)
		m_nextPushButton->setEnabled(false);
	/*exit page button*/
	m_exitPushButton = new QPushButton(QIcon(":/Resources/icon/exit.png"),QString::fromLocal8Bit("退出"),this);
	m_exitPushButton->setIconSize(QSize(200,40));
	/*configure page button*/
	m_configPushButton = new QPushButton(QString::fromLocal8Bit("配置菜单"),this);
	//default configure button
	m_defaultPushButton = new QPushButton(QStringLiteral("恢复初始设置"),this);

	connect(m_prevPushButton,SIGNAL(clicked()),this,SLOT(prevPushButton_clicked()));
	connect(m_nextPushButton,SIGNAL(clicked()),this,SLOT(nextPushButton_clicked()));
	connect(m_exitPushButton,SIGNAL(clicked()),this,SLOT(exitPushButton_clicked()));
	connect(m_configPushButton,SIGNAL(clicked()),this,SLOT(configDlgExec()));
	connect(m_defaultPushButton,SIGNAL(clicked()),this,SLOT(DefCfgPushButton_clicked()));
	connect(m_stackwidget,SIGNAL(widgetRemoved(int)),this,SLOT(setMaxPage()));
	connect(this,SIGNAL(pageChange()),this,SLOT(rePage()));
	resize(800,600);
}

void Scada::resizeEvent(QResizeEvent *e)
{
	m_prevPushButton->setGeometry(0,height()-50,120,50);
	m_nextPushButton->setGeometry(width()-120,height()-50,120,50);
	m_exitPushButton->setGeometry(width()/2-60,height()-50,120,50);
	m_configPushButton->setGeometry(120,height()-50,120,50);
	m_defaultPushButton->setGeometry(width()-240,height()-50,120,50);
	m_stackwidget->setGeometry(0,0,width(),height()-50);
	e->accept();
}

/* create main page*/
void Scada::initMainpage()
{
	insertPage();
	BasePage *mainpage = (BasePage *)m_stackwidget->widget(0);
	mainpage->setPageName(QStringLiteral("主页面"));
	mainpage->setRow(2);
	mainpage->setColumn(3);
	EngineMeter *enginemeter = new EngineMeter();
	FuelLevel *fuel = new FuelLevel();

	/* battery*/
	BaseMeter *battery = new BaseMeter("battery");
	battery->setMaxValue(30);
	battery->setStartAngle(60);
	battery->setEndAngle(60);
	battery->setUnits("V");
	battery->setTitle(QStringLiteral("电池电压"));
	battery->setScaleMajor(6);
	battery->setScaleMiddle(5);
	battery->setScaleMinor(5);
	battery->setScaleMinorLength(5);
	/*oidpress*/
	BaseMeter *oilpress = new BaseMeter("oilpress");
	oilpress->setMaxValue(2);
	oilpress->setStartAngle(120);
	oilpress->setEndAngle(120);
	oilpress->setUnits("Bar");
	oilpress->setTitle(QStringLiteral("油位"));
	oilpress->setScaleMajor(2);
	oilpress->setScaleMiddle(5);
	oilpress->setScaleMinor(10);
	oilpress->setShift(0.1);
	/*brakepress*/
	BaseMeter *brakepress = new BaseMeter("brakepress");
	brakepress->setMaxValue(30);
	brakepress->setUnits("Bar");
	brakepress->setTitle(QStringLiteral("制动压力"));
	brakepress->setScaleMajor(6);
	brakepress->setScaleMiddle(5);
	brakepress->setScaleMinor(5);
	brakepress->setScaleMinorLength(4);
	/*dbpress*/
	BaseMeter *dbpress = new BaseMeter("dbpress");
	dbpress->setMaxValue(200);
	dbpress->setUnits("Bar");
	dbpress->setTitle(QStringLiteral("双变压力"));
	dbpress->setScaleMajor(10);
	dbpress->setScaleMiddle(2);
	dbpress->setScaleMinor(4);
	dbpress->setShift(5);
	/*lamp*/
	IndicatorLamp *lamp[5];
	QStringList lampname;
	QStringList lamptitle;
	lamptitle << QStringLiteral("充电")<<QStringLiteral("行停车制动")<<QStringLiteral("发电机充电")<<QStringLiteral("制动压力")<<QStringLiteral("预热");
	lampname<<"chargelamp"<<"brake"<<"dynchargelamp"<<"uvchargelamp"<<"glowlamp";
	for(int i = 0;i<lampname.count();i++)
	{
		lamp[i]= new IndicatorLamp(lampname[i].toStdString().c_str());
		lamp[i]->setTitle(lamptitle[i]);
		mainpage->appendMeter(lamp[i],i+1);
	}
	mainpage->appendMeter(enginemeter,1);
	mainpage->appendMeter(fuel,2);
	mainpage->appendMeter(battery,3);
	mainpage->appendMeter(oilpress,4);
	mainpage->appendMeter(brakepress,4);
	mainpage->appendMeter(dbpress,6);
}

void Scada::insertPage(int num)
{
	for(int i = 0 ; i < num ; i++)
	{
		BasePage  *page = new BasePage();
		m_stackwidget->addWidget(page);
	}
	setMaxPage();
	emit pageChange();
}

void Scada::insertMeterToPage(int pagenum, QWidget *w, int pos)
{
	if(pagenum > m_maxpage)
	{
		QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("仪表插入页面超过最大页面，请重新选择页面").arg(m_maxpage).arg(pagenum));
		return ;
	}
	BasePage *page = (BasePage *)m_stackwidget->widget(pagenum-1);
	page->appendMeter((BaseParam*)w,pos);
}

/*write data to configrue file(filename)*/
void Scada::writeSettings(QString filename)
{
	Configure config(filename);
	config.clear();
	BaseMeter *meter;
	Thermometer *therm;
	IndicatorLamp *lamp;
	config.configureWrite(m_maxpage,"PageNum");
	
	for(int i=0 ; i< m_maxpage ; i++)  //page
	{
		QStringList widgetname;
		BasePage *page = (BasePage *)m_stackwidget->widget(i);
		int meternum = page->Counts();
		QString pageflag = QString("/Page%1/").arg(i+1);
		config.configureWrite(page->Row(),pageflag+"row");
		config.configureWrite(page->Colomn(),pageflag+"column");
		config.configureWrite(page->pageName(),pageflag+"PageName");
		config.configureWrite(meternum,pageflag+"widgetsNum");
		config.configureWrite(page->LampCounts(),pageflag+"LampNum");
		config.configureWrite("",pageflag+"widgetsName");
		for(int j=1;j <= meternum;j++)
		{
			BaseParam *basewidget = (BaseParam *)page->GetMeter(j-1);
			widgetname<<QString(basewidget->getName());
			QString widgetflag = pageflag+QString(basewidget->getName())+"/";
			config.configureWrite(basewidget->getName(),widgetflag+"name");
			config.configureWrite(j,widgetflag+"pos");
			switch(basewidget->getType())
			{
			case 1:
				meter = (BaseMeter *)basewidget;
				config.configureWrite("Meter",widgetflag+"type");
				config.configureWrite(meter->title(),widgetflag+"title");
				config.configureWrite(meter->units(),widgetflag+"unit");
				config.configureWrite(meter->getComponents(),widgetflag+"component");
				config.configureWrite(meter->minValue(),widgetflag+"minValue");
				config.configureWrite(meter->maxValue(),widgetflag+"maxValue");
				config.configureWrite(meter->startAngle(),widgetflag+"startAngle");
				config.configureWrite(meter->endAngle(),widgetflag+"endAngle");
				config.configureWrite(meter->scaleMajor(),widgetflag+"scaleMajor");
				config.configureWrite(meter->scaleMiddle(),widgetflag+"scaleMiddle");
				config.configureWrite(meter->scaleMinor(),widgetflag+"scaleMinor");
				config.configureWrite(meter->scaleMajorLength(),widgetflag+"scaleMajorLength");
				config.configureWrite(meter->scaleMiddleLength(),widgetflag+"scaleMiddleLength");
				config.configureWrite(meter->scaleMinorLength(),widgetflag+"scaleMinorLength");
				break;
			case 2:
				therm = (Thermometer*)basewidget;
				config.configureWrite("Thermometer",widgetflag+"type");
				config.configureWrite(therm->title(),widgetflag+"title");
				config.configureWrite(therm->units(),widgetflag+"unit");
				config.configureWrite(therm->minValue(),widgetflag+"minValue");
				config.configureWrite(therm->maxValue(),widgetflag+"maxValue");
				config.configureWrite(therm->steps(),widgetflag+"steps");
				break;
			case 3:
				lamp = (IndicatorLamp*)basewidget;
				config.configureWrite("IndicatorLamp",widgetflag+"type");
				config.configureWrite(lamp->title(),widgetflag+"title");
				break;
			default:
				break;
			}
		}
		config.configureWrite(widgetname,pageflag+"widgetsName");
	}
}

/*read data from configure file(filename)*/
void Scada::readSettings(QString filename)
{
	Configure config(filename);
	int pagenum = config.configureRead("PageNum").toInt(); 
	while(m_maxpage)
	{
		m_stackwidget->removeWidget(m_stackwidget->widget(0));
	}
	insertPage(pagenum);
	for(int i=0;i<pagenum;i++)
	{
		BasePage *page =(BasePage*) m_stackwidget->widget(i);
		QString pageflag = QString("/Page%1/").arg(i+1);
		page->setRow(config.configureRead( pageflag + "row").toInt());
		page->setColumn(config.configureRead( pageflag+ "column").toInt());
		page->setPageName(config.configureRead(pageflag+"PageName").toByteArray().data());;
		int widgetnum = config.configureRead(pageflag+"widgetsNum").toInt();
		QStringList widgetsname = config.configureRead(pageflag + "widgetsName").toStringList();
		
		for(int j = 0;j<widgetnum;j++)
		{
			QString widgetflag = pageflag + widgetsname[j] + "/";
			QString type = config.configureRead(widgetflag + "type").toString();
			if(type == "Meter")
			{
				BaseMeter *meter;
				QString name = config.configureRead(widgetflag + "name").toString();
				if(name == "enginemeter")
					meter = new EngineMeter();
				else if(name == "fuellevel")
					meter = new FuelLevel();
				else
					meter = new BaseMeter(name.toStdString().c_str());

				meter->setTitle(config.configureRead(widgetflag+"title").toString());
				meter->setUnits(config.configureRead(widgetflag+"unit").toString());
				meter->setComponment(config.configureRead(widgetflag+"component").toInt());
				meter->setMinValue(config.configureRead(widgetflag+"minValue").toInt());
				meter->setMaxValue(config.configureRead(widgetflag+"maxValue").toInt());
				meter->setStartAngle(config.configureRead(widgetflag + "startAngle").toInt());
				meter->setEndAngle(config.configureRead(widgetflag + "endAngle").toInt());
				meter->setScaleMajor(config.configureRead(widgetflag + "scaleMajor").toInt());
				meter->setScaleMiddle(config.configureRead(widgetflag + "scaleMiddle").toInt());
				meter->setScaleMinor(config.configureRead(widgetflag + "scaleMinor").toInt());
				meter->setScaleMajorLength(config.configureRead(widgetflag + "scaleMajorLength").toInt());
				meter->setScaleMiddleLength(config.configureRead(widgetflag + "scaleMiddleLength").toInt());
				meter->setScaleMinorLength(config.configureRead(widgetflag + "scaleMinorLength").toInt());
				page->appendMeter(meter,config.configureRead(widgetflag+"pos").toInt());		
			}
			if(type == "Thermometer")
			{
				Thermometer *therm = new Thermometer(config.configureRead(widgetflag + "name").toString().toStdString().c_str());
				therm->setTitle(config.configureRead(widgetflag+"title").toString());
				therm->setMinValue(config.configureRead(widgetflag+"minValue").toInt());
				therm->setMaxValue(config.configureRead(widgetflag+"maxValue").toInt());
				therm->setSteps(config.configureRead(widgetflag+"steps").toInt());
				page->appendMeter(therm,config.configureRead(widgetflag+"pos").toInt());
			}
			if(type == "IndicatorLamp")
			{
				IndicatorLamp *lamp = new IndicatorLamp(config.configureRead(widgetflag + "name").toString().toStdString().c_str());
				lamp->setTitle(config.configureRead(widgetflag+"title").toString());
				page->appendMeter(lamp,config.configureRead(widgetflag+"pos").toInt());
			}
		}
	}
	update();
}

/*private slots */
void Scada::setMaxPage()
{
	m_maxpage = m_stackwidget->count();
}

void Scada::rePage()
{
	if(m_maxpage <=1)
	{
		m_prevPushButton->setEnabled(false);
		m_nextPushButton->setEnabled(false);
	}
	else
	{ 
		if(m_currentPage == 0)
		{
			m_prevPushButton->setEnabled(false);
			m_nextPushButton->setEnabled(true);
		}
		else if(m_currentPage == m_maxpage-1)
		{
			m_prevPushButton->setEnabled(true);
			m_nextPushButton->setEnabled(false);
		}
		else
		{
			m_prevPushButton->setEnabled(true);
			m_nextPushButton->setEnabled(true);
		}
	}
	m_stackwidget->setCurrentIndex(m_currentPage);
}

void Scada::prevPushButton_clicked()
{
	
	if(m_currentPage > 0)
		--m_currentPage;
	if(m_currentPage == 0)
		m_prevPushButton->setEnabled(false);
	else
		m_prevPushButton->setEnabled(true);
	m_nextPushButton->setEnabled(true);
	m_stackwidget->setCurrentIndex(m_currentPage);
}

void Scada::nextPushButton_clicked()
{
	if(m_currentPage < m_stackwidget->count()-1)
		++m_currentPage;
	if(m_currentPage == m_stackwidget->count()-1 )
		m_nextPushButton->setEnabled(false);
	else
		m_nextPushButton->setEnabled(true);
	m_prevPushButton->setEnabled(true);
	m_stackwidget->setCurrentIndex(m_currentPage);
}

/*恢复初始设置*/
void Scada::DefCfgPushButton_clicked()
{
	QMessageBox msgBox;
	//msgBox.setText(QStringLiteral("恢复初始设置"));
	msgBox.setInformativeText(QStringLiteral("恢复到默认设置可能会丢失数据，是否继续执行？"));
	msgBox.setStandardButtons(QMessageBox::Ok  | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	if (msgBox.exec() == QMessageBox::Ok)
		readSettings("default.ini");
}
//full screen
void Scada::mouseDoubleClickEvent(QMouseEvent *event)
{
	
	if(event->button() == Qt::LeftButton)
	{
		if(!isFullScreen())
			showFullScreen();
		else
			showNormal();
		event->accept();
	}
}

/*exit*/
void Scada::exitPushButton_clicked()
{
	close();  //invoke closeEvent()
}

void Scada::closeEvent(QCloseEvent *e)
{

#if 0
#if DEFAULT
	writeSettings(configfile);
#else
	writeSettings();
#endif
#endif

	e->accept();
}

void Scada::configDlgAccepted()
{
	//QMessageBox::about(this,"accepted","accepted");
	readSettings("config.ini");
}

void Scada::configDlgExec()
{
	m_configDlg = new QConfigDlg(this);
	m_configDlg->exec();
}

/* testing
** create the second page
*/
void Scada::test()
{
	insertPage();
	int i = m_maxpage;
	Thermometer *oiltemp = new Thermometer("oiltemp");
	oiltemp->setMinValue(0);
	oiltemp->setMaxValue(200);
	oiltemp->setSteps(20);
	oiltemp->setTitle(QStringLiteral("机油温度"));
	Thermometer *cyltemp = new Thermometer("cyltemp");
	cyltemp->setMinValue(0);
	cyltemp->setMaxValue(200);
	cyltemp->setSteps(20);
	cyltemp->setTitle(QStringLiteral("缸盖温度"));
	Thermometer *dbltemp = new Thermometer("dbltemp");
	dbltemp->setMinValue(0);
	dbltemp->setMaxValue(200);
	dbltemp->setSteps(20);
	dbltemp->setTitle(QStringLiteral("双变温度"));

	insertMeterToPage(i,oiltemp,1);
	insertMeterToPage(i,cyltemp,2);
	insertMeterToPage(i,dbltemp,3);
}
