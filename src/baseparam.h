#ifndef BASEPARAM_H
#define BASEPARAM_H

#include <QWidget>

class BaseParam : public QWidget
{
    Q_OBJECT
public:
    enum Type {
        Meter		= 1,     //����
        Thermometer	= 2,     //�¶ȼ�
        Lamp		= 3      //ָʾ��
    };
    BaseParam(QWidget *parent =0,const char* keyname = "name",int type = Type::Meter);
    virtual ~BaseParam();
    const char* getName() const {return m_keyName;}
    int getType() const {return m_type;}
    virtual void paintEvent(QPaintEvent *) = 0;
public Q_SLOTS:
    virtual void setName(const char *name);
    virtual void setType(int type);
protected:
    char m_keyName[50];
    int m_type;
};

#endif // BASEPARAM_H
