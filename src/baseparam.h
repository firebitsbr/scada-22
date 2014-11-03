
/****************************************
**	File Name:	baseparam.h
**  Description:
**	BaseParam is an abstract base class,saving the basic properties of meters 
**	including meter type and meter name.
**	All meter class (such as BaseMeter) must be inherited from class BaseParam.
**  Author:		teddy.tu(tudongdong1989@gmail.com)
**	Date:		2014/10/1
**	Remark:		teddy.tu
**  Date:		2014/10/1
**	Version:		1.0.0
**
**  Copyright(C) teddy.tu
**  All rights reserved.
****************************************/ 
#ifndef BASEPARAM_H
#define BASEPARAM_H

#include <QWidget>

class BaseParam : public QWidget
{
    Q_OBJECT
public:
	
	//enumerate variable of meter type
    enum Type {
        Meter		= 1,     //dial
        Thermometer	= 2,     //thermo-graph
        Lamp		= 3      //pilot lamp
    };
    BaseParam(QWidget *parent =0,const char* keyname = "name",int type = Type::Meter);
    virtual ~BaseParam();
    const char* getName() const {return m_keyName;}		//get meter name
    int getType() const {return m_type;}			//get meter type
    virtual void paintEvent(QPaintEvent *) = 0;
public Q_SLOTS:
    virtual void setName(const char *name);
    virtual void setType(int type);
protected:
    char m_keyName[50];		//meter name
    int m_type;				//meter type ,set by enumerate variable Type 
};

#endif // BASEPARAM_H
