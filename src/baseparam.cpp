#include "baseparam.h"

BaseParam::BaseParam(QWidget *parent, const char *keyname, int type):
    QWidget(parent),m_type(type)
{
   /* if(type == Type::Meter)
        qstrncpy(m_keyName,"meter",50);
    else if(type == Type::Thermometer)
        qstrncpy(m_keyName,"thermometer",50);
    else if(type == Type::Lamp)
        qstrncpy(m_keyName,"Lamp",50);
    else*/
        qstrncpy(m_keyName,keyname,50);

}

BaseParam::~BaseParam()
{
}

void BaseParam::setName(const char *name)
{
    qstrncpy(m_keyName, name,50);
}
void BaseParam::setType(int type)
{
    m_type = type;
}




