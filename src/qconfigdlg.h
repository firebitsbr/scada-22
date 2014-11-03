#ifndef QCONFIGDLG_H
#define QCONFIGDLG_H

#include <QDialog>
#include <qmap.h>
#include <qstackedwidget.h>
#include <qtreewidget.h>

class QListWidget;
class QGroupBox;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QLabel;
class QPushButton;


class QConfigDlg : public QDialog
{
	Q_OBJECT

public:
	QConfigDlg(QWidget *parent);
	~QConfigDlg();

public Q_SLOTS:

	void treewidgetclicked(QTreeWidgetItem *item);  //
	
	void ShowPushbuttonClicked();	
	void SavePushbuttonClicked();
	void CancelPushbuttonClicked();
	
	//page setting
	void setColumnVal(int );
	void setRowVal(int);
	
	void setMeterTitle(const QString);
	void setUnit(const QString);
	void setPos(int);
	void setMeterType(int);
	void setMaxValue(const QString);
	void setMinValue(const QString);
	void Changed(bool flag);

private:
	void createUI();
	void initTreewidget();
	void closeEvent(QCloseEvent *);
	bool searchfile(QString filename,QString dir = "./");	
	bool copyfile(QString filename1,QString filename2);

	QSpinBox *m_columnSpinBox,*m_rowSpinBox,*m_posSpinBox;
	QComboBox *m_styleComboBox;
	QPushButton *m_styleEditPushButton;
	QPushButton *m_ShowPushbutton,*m_SavePushbutton,*m_CancelPushbutton;
	QLineEdit *m_datatypeLineEdit,*m_minvalueLineEdit,*m_maxvalueLineEdit,*m_unitLineEdit;
	QGroupBox *m_PageStyleGroupbox,*m_WidgetStyleGroupbox,*m_RangeGroupbox;
	bool m_dataChanged;


	QTreeWidget *m_treewidget;
	QMap<QTreeWidgetItem *,QString> m_meter; //保存控件的名称
	QListWidget *m_listwidget;


signals:
	void datachanged(bool flag );
};

#endif // QCONFIGDLG_H
