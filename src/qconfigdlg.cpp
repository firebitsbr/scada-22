#include "qconfigdlg.h"
#include <qlistwidget.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qtreewidget.h>
#include <qevent.h>
//#include <qlist.h>
#include <qstringlist.h>
#include "configure.h"
#include "init.h"
#include "scada.h"
//QMap<QString,QWidget*> QConfigDlg::m_map = QMap<QString,QWidget*>();
QConfigDlg::QConfigDlg(QWidget *parent)
	: QDialog(parent)
{
	m_dataChanged = false;  //��¼�Ƿ��������
	m_treewidget = new QTreeWidget();
	setAttribute(Qt::WA_DeleteOnClose);
	initTreewidget();
	createUI();
	copyfile(configfile,configbakfile);
}

QConfigDlg::~QConfigDlg()
{

}

void QConfigDlg::createUI()
{
	/* ҳ����ʽ����*/
	m_rowSpinBox = new QSpinBox(this);
	m_rowSpinBox->setRange(1,3);
	m_rowSpinBox->setAlignment(Qt::AlignCenter);
	m_columnSpinBox = new QSpinBox(this);
	m_columnSpinBox->setRange(1,4);
	m_columnSpinBox->setValue(1);
	m_columnSpinBox->setAlignment(Qt::AlignCenter);
	QHBoxLayout *toplayout = new QHBoxLayout;
	toplayout->addWidget(m_rowSpinBox);
	toplayout->addWidget((QLabel*) (new QLabel(QString::fromLocal8Bit("�� �� "),this)));
	toplayout->addWidget(m_columnSpinBox);
	toplayout->addWidget((QLabel*) (new QLabel(QString::fromLocal8Bit("��"),this)));
	m_PageStyleGroupbox = new QGroupBox(QString::fromLocal8Bit("ҳ����ʽ����"),this);
	m_PageStyleGroupbox->setLayout(toplayout);
	m_PageStyleGroupbox->setEnabled(false);

	/*�Ǳ���ʽ����*/
	m_datatypeLineEdit = new QLineEdit(this);
	m_posSpinBox = new QSpinBox(this);
	//m_posSpinBox->setRange(1,m_columnSpinBox->value() * m_rowSpinBox->value());
	m_styleComboBox = new QComboBox(this);
	m_styleComboBox->addItem(QString::fromLocal8Bit("����"));
	m_styleComboBox->addItem(QString::fromLocal8Bit("�¶ȼ�"));
	m_styleComboBox->addItem(QString::fromLocal8Bit("����ָʾ��"));
	m_styleEditPushButton = new QPushButton(QString::fromLocal8Bit("�Զ���"),this);
	QLabel *datatypeLabel = new QLabel(QString::fromLocal8Bit("�Ǳ�����"),this);
	QLabel *posLabel = new QLabel(QString::fromLocal8Bit("��ʾλ��"),this);
	QLabel *styltLabel = new QLabel(QString::fromLocal8Bit("��ʾ��ʽ"),this);
	QLabel *unitLabel = new QLabel(QString::fromLocal8Bit("��λ"),this);
	m_unitLineEdit = new QLineEdit(this);
	m_styleComboBox->setEnabled(false);
	m_styleEditPushButton->setEnabled(false);

	QGridLayout *middlelayout = new QGridLayout;
	middlelayout->addWidget(datatypeLabel,0,0);					/*����*/
	middlelayout->addWidget(m_datatypeLineEdit,0,1,1,1);
	middlelayout->addWidget(unitLabel,1,0,1,1);					/*��λ*/
	middlelayout->addWidget(m_unitLineEdit,1,1,1,1);
	middlelayout->addWidget(posLabel,2,0);						/*λ��*/
	middlelayout->addWidget(m_posSpinBox,2,1,1,1);
	middlelayout->addWidget(styltLabel,3,0);					/*��ʾ��ʽ*/
	middlelayout->addWidget(m_styleComboBox,3,1);
	middlelayout->addWidget(m_styleEditPushButton,3,2);
	m_WidgetStyleGroupbox = new QGroupBox(QString::fromLocal8Bit("�Ǳ���ʾ��ʽ����"),this);
	m_WidgetStyleGroupbox->setLayout(middlelayout);
	m_WidgetStyleGroupbox->setEnabled(false);

	/*��������*/
	QLabel *minvalueLabel = new QLabel(QString::fromLocal8Bit("��Сֵ"),this);
	QLabel *maxvalueLabel = new QLabel(QString::fromLocal8Bit("���ֵ"),this);
	m_minvalueLineEdit = new QLineEdit(this);
	m_maxvalueLineEdit = new QLineEdit(this);
	QGridLayout *m_ranglayout = new QGridLayout;
	m_ranglayout->addWidget(minvalueLabel,0,0);
	m_ranglayout->addWidget(m_minvalueLineEdit,0,1);
	m_ranglayout->addWidget(maxvalueLabel,1,0);
	m_ranglayout->addWidget(m_maxvalueLineEdit,1,1);
	m_RangeGroupbox = new QGroupBox(QString::fromLocal8Bit("���̷�Χ����"),this);
	m_RangeGroupbox->setLayout(m_ranglayout);
	m_RangeGroupbox->setEnabled(false);

	QVBoxLayout *rightlayout = new QVBoxLayout;
	rightlayout->addWidget(m_PageStyleGroupbox);
	rightlayout->addWidget(m_WidgetStyleGroupbox);
	rightlayout->addWidget(m_RangeGroupbox);
	/*����*/
	m_ShowPushbutton = new QPushButton(QString::fromLocal8Bit("Ԥ��"));
	m_ShowPushbutton->setEnabled(false);
	m_SavePushbutton = new QPushButton(QString::fromLocal8Bit("����"));
	m_CancelPushbutton = new QPushButton(QString::fromLocal8Bit("�˳�"));
	QHBoxLayout *downlayout = new QHBoxLayout;
	downlayout->addWidget(m_ShowPushbutton);
	downlayout->addWidget(m_SavePushbutton);
	downlayout->addWidget(m_CancelPushbutton);

	QHBoxLayout *uplayout = new QHBoxLayout;
	uplayout->addWidget(m_treewidget);
	uplayout->addLayout(rightlayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(uplayout);
	layout->addStretch();
	layout->addLayout(downlayout);
	setLayout(layout);
	setWindowTitle(QString::fromLocal8Bit("����"));
	
	connect(m_treewidget,SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(treewidgetclicked(QTreeWidgetItem *)));
	connect(m_ShowPushbutton,SIGNAL(clicked()),this,SLOT(ShowPushbuttonClicked()));
	connect(m_SavePushbutton,SIGNAL(clicked()),this,SLOT(SavePushbuttonClicked()));
	connect(m_CancelPushbutton,SIGNAL(clicked()),this,SLOT(CancelPushbuttonClicked()));
	//connect(this,SIGNAL(Accepted()),this,SLOT(accept()));
	connect(this,SIGNAL(datachanged(bool )),this,SLOT(Changed(bool))); //data changed
	connect(m_rowSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setRowVal(int)));
	connect(m_columnSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setColumnVal(int)));
	connect(m_datatypeLineEdit,SIGNAL(textEdited(const QString)),this,SLOT(setMeterTitle(const QString)));
	connect(m_unitLineEdit,SIGNAL(textEdited(const QString)),this,SLOT(setUnit(const QString)));
	connect(m_posSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setPos(int)));
	connect(m_minvalueLineEdit,SIGNAL(textEdited(const QString)),this,SLOT(setMinValue(const QString)));
	connect(m_maxvalueLineEdit,SIGNAL(textEdited(const QString)),this,SLOT(setMaxValue(const QString)));

	setMinimumSize(500,400);
	resize(500,400);
}

bool QConfigDlg::searchfile(QString filename, QString dirname)
{
	QDir dir(dirname);
	if(dir.exists())
	{
		dir.setFilter(QDir::Files);
		QFileInfoList filelist = dir.entryInfoList();
		for(int i=0;i<filelist.size();i++)
		{

		}
		QMessageBox::about(0,"dir","exist");
		
	}
	else
	{
		QMessageBox::about(0,"dir","not exist");
		return false;
	}	
	return true;
}

/*copy filename1 to filename2*/
bool QConfigDlg::copyfile(QString filename1,QString filename2)
{
	QFile srcfile(filename2);
	if(srcfile.open(QIODevice::ReadOnly))
		srcfile.remove();
	return QFile::copy(filename1,filename2);
}

/*
**	�����ñ��ж�ȡ�Ǳ���ʼ��������ʾ�б�
*/
void QConfigDlg::initTreewidget()
{
	m_treewidget->setColumnCount(1);
	m_treewidget->setHeaderLabel(QString::fromLocal8Bit("�Ǳ�Ŀ¼"));
	QStringList page_name,widget_name;
	QTreeWidgetItem *page,*widget;

	Configure config("config.ini");
	int pagenum = config.configureRead("PageNum").toInt(); 
	for(int i=0;i<pagenum;i++)
	{
		page_name.clear();
		QString pageflag = QString("/Page%1/").arg(i+1);
		page_name<<config.configureRead(pageflag + "PageName").toString();
		page = new QTreeWidgetItem(m_treewidget,page_name);
		m_meter[page] = QString("Page%1").arg(i+1);
		int widgetnum = config.configureRead(pageflag + "widgetsNum").toInt();
		QStringList widgetsname = config.configureRead(pageflag + "widgetsName").toStringList();
		for(int j = 0; j<widgetnum ; j++)
		{
			QString widgetflag = pageflag + widgetsname[j] + "/";
			widget_name.clear();
			widget_name << config.configureRead(widgetflag + "/title").toString();
			widget = new QTreeWidgetItem(page,widget_name);
			page->addChild(widget);
			m_meter[widget] = config.configureRead(widgetflag + "/name").toString();
		}
	}
}

/*public slots*/
void QConfigDlg::treewidgetclicked(QTreeWidgetItem *item)
{
	Configure config(configbakfile);

	if(item->parent())		//select widgets 
	{
		m_PageStyleGroupbox->setTitle(item->parent()->text(0)+QString::fromLocal8Bit("��ʽ����"));
		m_PageStyleGroupbox->setEnabled(false);
		m_WidgetStyleGroupbox->setEnabled(true);
		m_RangeGroupbox->setEnabled(true);
		QMap<QTreeWidgetItem *,QString>::iterator iter;
		if((iter = m_meter.find(item->parent())) != m_meter.end())
		{
			QString page = iter.value();
			m_rowSpinBox->setValue(config.configureRead("row",page).toInt());
			m_columnSpinBox->setValue(config.configureRead("column",page).toInt());
			int widgetsnum = config.configureRead("widgetsNum",page).toInt();
			int lampnum = config.configureRead("LampNum",page).toInt();
			//m_posSpinBox->setRange(1,widgetsnum);
			if((iter = m_meter.find(item)) != m_meter.end())
			{
				QString widgetname = iter.value();
				QString type = config.configureRead("type",widgetname,page).toString();
				QString style = item->text(0);
				if(type == "Meter")
				{
					m_styleComboBox->setCurrentIndex(0);
					m_posSpinBox->setRange(1,widgetsnum-lampnum);
				}
				else if(type == "Thermometer")
				{
					m_styleComboBox->setCurrentIndex(1);
					m_posSpinBox->setRange(1,widgetsnum-lampnum);
				}
				else
				{
					m_styleComboBox->setCurrentIndex(2);
					m_posSpinBox->setRange(widgetsnum-lampnum+1,widgetsnum);
				}
				m_datatypeLineEdit->setText(style);
				
				QString unit = config.configureRead("unit",widgetname,page).toString();
				m_unitLineEdit->setEnabled(!unit.isEmpty());	
				m_unitLineEdit->setText(unit);
		
				int pos = config.configureRead("pos",widgetname,page).toInt();
				m_posSpinBox->setValue(pos);

				QString minvalue = config.configureRead("minValue",widgetname,page).toString();
				m_minvalueLineEdit->setEnabled(!minvalue.isEmpty());
				m_minvalueLineEdit->setText(minvalue);

				QString maxvalue = config.configureRead("maxValue",widgetname,page).toString();
				m_maxvalueLineEdit->setEnabled(!maxvalue.isEmpty());
				m_maxvalueLineEdit->setText(maxvalue);
			}
		}
	}
	else		//select page
	{
		m_PageStyleGroupbox->setTitle(item->text(0)+QString::fromLocal8Bit("��ʽ����"));
		m_PageStyleGroupbox->setEnabled(true);
		m_WidgetStyleGroupbox->setEnabled(false);
		m_RangeGroupbox->setEnabled(false);
		
		QMap<QTreeWidgetItem *,QString>::iterator iter;
		if((iter = m_meter.find(item))!=m_meter.end())
		{
			m_rowSpinBox->setValue(config.configureRead("row",iter.value()).toInt());
			m_columnSpinBox->setValue(config.configureRead("column",iter.value()).toInt());
		}
	}
}

//Ԥ����ť
void QConfigDlg::ShowPushbuttonClicked()
{
	QMessageBox::about(0,"show","Disabled");
}

//���水ť
void QConfigDlg::SavePushbuttonClicked()
{
	Configure filebak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	while(item->parent())
		item = item->parent();
	QFile file1(configfile); 
	QFile file(configbakfile);
	if(m_dataChanged)
	{
		int rowval = filebak.configureRead("row",m_meter[item]).toInt();
		int columnval = filebak.configureRead("column",m_meter[item]).toInt();
		int widgetnum = filebak.configureRead("widgetsNum",m_meter[item]).toInt();
		int lampnum = filebak.configureRead("LampNum",m_meter[item]).toInt();
		int meternum = widgetnum - lampnum;
		if(rowval*columnval < meternum)
		{
			QMessageBox::warning(0,QStringLiteral("����"),QStringLiteral("�ᵼ�²����Ǳ�����ʾ������������ҳ����ʾ��ʽ(��Ҫ��ʾ%1���Ǳ������ֻ����ʾ%2���Ǳ�)").arg(meternum).arg(rowval*columnval));
			return;
		}
		else
		{
			 //��ɾ��ԭʼ�ļ�
			file1.remove();
			 //���������ļ�
			file.rename(configfile);

			copyfile(configfile,configbakfile); //���������ļ�
			m_dataChanged = false;
		}
	}
	QMessageBox::about(0,QStringLiteral("����"),QStringLiteral("���ݱ���ɹ���"));
}

//�˳����ý��水ť
void QConfigDlg::CancelPushbuttonClicked()
{
	this->close();
}

void QConfigDlg::closeEvent(QCloseEvent *e)
{
	Configure filebak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	while(item->parent())
		item = item->parent();
	if(!m_dataChanged)//û�и�������
	{
		QFile file(configbakfile);
		file.remove();
	}
	else
	{
		int rowval = filebak.configureRead("row",m_meter[item]).toInt();
		int columnval = filebak.configureRead("column",m_meter[item]).toInt();
		int widgetnum = filebak.configureRead("widgetsNum",m_meter[item]).toInt();
		int lampnum = filebak.configureRead("LampNum",m_meter[item]).toInt();
		int meternum = widgetnum - lampnum;
		if(rowval*columnval < meternum)
		{
			QMessageBox::warning(0,QStringLiteral("����"),QStringLiteral("�ᵼ�²����Ǳ�����ʾ������������ҳ����ʾ��ʽ(��Ҫ��ʾ%1���Ǳ������ֻ����ʾ%2���Ǳ�)").arg(meternum).arg(rowval*columnval));
			e->ignore();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("�Ǳ���������Ѹ���"));
			msgBox.setInformativeText(QStringLiteral("�Ƿ񱣴���Ĳ���?"));
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			QFile file1(configfile);
			QFile file(configbakfile);
			switch(msgBox.exec())
			{
			case QMessageBox::Save :
				file1.remove();
				file.rename(configfile);
				break;
			case QMessageBox::Cancel :
				file.remove();
				break;
			}
			e->accept();
		}
	}
	Scada *scada = (Scada*)parentWidget();
	scada->configDlgAccepted();
}

//���ݸ��ı�־
void QConfigDlg::Changed(bool flag)
{
	m_dataChanged = flag;
}

//ҳ��������
void QConfigDlg::setRowVal(int val)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	while(item->parent())
		item = item->parent();
	int rowval = config.configureRead("row",m_meter[item]).toInt();
 	if(val != rowval)
	{
		m_rowSpinBox->setValue(val);
		configbak.configureWrite(val,"row",m_meter[item]);
		emit(datachanged(true));
	}
	else
		emit datachanged(false);
}

// ҳ�������� 
void QConfigDlg::setColumnVal(int val)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	while(item->parent())
		item = item->parent();
	int columnval = config.configureRead("column",m_meter[item]).toInt();
 	if(val != columnval)
	{
		m_columnSpinBox->setValue(val);
		configbak.configureWrite(val,"column",m_meter[item]);
		emit(datachanged(true));
	}
	else
		emit datachanged(false);
}

//set meter name
void QConfigDlg::setMeterTitle(const QString title)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	QTreeWidgetItem *pitem = item->parent();
	QString titleval = config.configureRead("title",m_meter[item],m_meter[pitem]).toString();
 	if(title != titleval)
	{
		m_datatypeLineEdit->setText(title);
		configbak.configureWrite(title,"title",m_meter[item],m_meter[pitem]);
		emit datachanged(true);
	}
	else
		emit datachanged(false);
}
//set units
void QConfigDlg::setUnit(const QString unit)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	QTreeWidgetItem *pitem = item->parent();
	QString unitval = config.configureRead("unit",m_meter[item],m_meter[pitem]).toString();
	if(unit != unitval)
	{
		m_unitLineEdit->setText(unit);
		configbak.configureWrite(unit,"unit",m_meter[item],m_meter[pitem]);
		emit(datachanged(true));
	}
	else
		emit datachanged(false);
}

void QConfigDlg::setPos(int pos)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	QTreeWidgetItem *pitem = item->parent();
	int originpos = config.configureRead("pos",m_meter[item],m_meter[pitem]).toInt();
	int oldpos = configbak.configureRead("pos",m_meter[item],m_meter[pitem]).toInt();
	QString type = configbak.configureRead("type",m_meter[item],m_meter[pitem]).toString();
	int lampnum = configbak.configureRead("LampNum",m_meter[pitem]).toInt();
	int meternum = configbak.configureRead("widgetsNum",m_meter[pitem]).toInt()-lampnum;
	if(pos != oldpos)
	{
		if((type == "Meter" || type == "Thermometer") && (pos>meternum ||pos<=0))
		{
			QMessageBox::warning(0,QStringLiteral("λ������"),QStringLiteral("��������ȷ��λ��(%1~%2)").arg(1).arg(meternum));
			return;
		}
		if(type == "IndicatorLamp" && (pos<=meternum || pos>meternum+lampnum))
		{	
			QMessageBox::warning(0,QStringLiteral("λ������"),QStringLiteral("��������ȷ��λ��(%1~%2)").arg(meternum+1).arg(meternum+lampnum));
			return;
		}
		QStringList strlist = config.configureRead("widgetsName",m_meter[pitem]).toStringList();
		for(int i=0;i<strlist.length();i++)
		{
			int rpos = configbak.configureRead("pos",strlist[i],m_meter[pitem]).toInt();
			if( pos<oldpos  &&  pos <= rpos && rpos < oldpos) //��ǰ��
			{
				configbak.configureWrite(rpos+1,"pos",strlist[i],m_meter[pitem]);
				continue;
			}
			 if( pos>oldpos && oldpos < rpos && rpos <= pos) //�����
			 {
				configbak.configureWrite(rpos-1,"pos",strlist[i],m_meter[pitem]);
				continue;
			 }
		}
		m_posSpinBox->setValue(pos);
		configbak.configureWrite(pos,"pos",m_meter[item],m_meter[pitem]);
		
	}
	if(pos != originpos)
		emit datachanged(true);
	else
		emit datachanged(false);
}

void QConfigDlg::setMeterType(int type)
{

}

void QConfigDlg::setMinValue(const QString val)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	QTreeWidgetItem *pitem = item->parent();
	QString  minval = config.configureRead("minValue",m_meter[item],m_meter[pitem]).toString();
 	if(val != minval)
	{
		m_minvalueLineEdit->setText(val);
		configbak.configureWrite(val,"minValue",m_meter[item],m_meter[pitem]);
		emit datachanged(true);
	}
	else
		emit datachanged(false);
}

void QConfigDlg::setMaxValue(const QString val)
{
	Configure config(configfile);
	Configure configbak(configbakfile);
	QTreeWidgetItem *item = m_treewidget->currentItem();
	QTreeWidgetItem *pitem = item->parent();
	QString  maxval = config.configureRead("maxValue",m_meter[item],m_meter[pitem]).toString();
 	if(val != maxval)
	{
		m_maxvalueLineEdit->setText(val);
		configbak.configureWrite(val,"maxValue",m_meter[item],m_meter[pitem]);
		emit datachanged(true);
	}
	else
		emit datachanged(false);
}