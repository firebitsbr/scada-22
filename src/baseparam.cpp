
/****************************************
**	File Name:	baseparam.cpp
**  Description:
**	Implement of BaseParam.
**  Author:		teddy.tu(tudongdong1989@gmail.com)
**	Date:		2014/10/1
**	Remark:		teddy.tu
**  Date:
**	Version:		1.0.0
**
**  Copyright(C) teddy.tu
**  All rights reserved.
****************************************/ 
#include "baseparam.h"

BaseParam::BaseParam(QWidget *parent, const char *keyname, int type):
    QWidget(parent),m_type(type)
{
   /*
   if(type == Type::Meter)
        qstrncpy(m_keyName,"meter",50);
    else if(type == Type::Thermometer)
        qstrncpy(m_keyName,"thermometer",50);
    else if(type == Type::Lamp)
        qstrncpy(m_keyName,"Lamp",50);
    else
	*/
        qstrncpy(m_keyName,keyname,50);

}

BaseParam::~BaseParam()
{
}

/*
**  @description:
**	Set meter name
**  @parameter
**	@param name : the new name for set meter name
**  @return value:NULL
**
**  author:teddy.tu
**  date:	2014/10/1
**
**  Copyright(C) teddy.tu
**  All rights reserved.
*/ 
void BaseParam::setName(const char *name)
{
    qstrncpy(m_keyName, name,50);
}
/*
**  @description:
**	Set meter type
**  @parameter
**	@param name : the meter type, values set depend on  enumerate variable Type
**  @return value:NULL
**
**  author:teddy.tu
**  date:	2014/10/1
**
**  Copyright(C) teddy.tu
**  All rights reserved.
*/ 
void BaseParam::setType(int type)
{
    m_type = type;
}




