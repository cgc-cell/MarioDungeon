#include "storewidget.h"

StoreWidget::StoreWidget(QWidget *parent) : QWidget(parent)
{
    initialStoreWin();
    //StoreWin->setFocus();//构造函数中设置焦点为当前窗口无用，因为窗口还没显示出来
}
void StoreWidget::initialStoreWin()
{
    StoreWin=new QWidget(this);//类中不定义一个主窗口，那么新建的类对象也会没有主窗口
    QFont font("Microsoft YaHei" ,12, 30);
    this->setFont(font);
    StoreWin->setStyleSheet("color:white;background-color:black");

    fac=new Factory();
    QLabel *Store_pic=fac->CreateQLabel(StoreWin,10,10,30,30,"","border-image: url(:/info/image/information/Store.png);");
    text1=fac->CreateQLabel(StoreWin,50,10,192,30,"你若给我 40 个金币");
    text2=fac->CreateQLabel(StoreWin,50,50,192,30,"我就可以帮你");
    hpOption=fac->CreateQLabel(StoreWin,32,112,192,32,"提升 100 点生命");
    mpOption=fac->CreateQLabel(StoreWin,32,160,192,32,"提升 20 点魔法");
    atkOption=fac->CreateQLabel(StoreWin,32,208,192,32,"提升 2 点攻击力");
    defOption=fac->CreateQLabel(StoreWin,32,256,192,32,"提升 4 点防御力");
    exitOption=fac->CreateQLabel(StoreWin,32,304,192,32,"离开商店");

    OptionBox=new QGraphicsView(StoreWin);//最后构造的控件会在最上面
    OptionBox->setStyleSheet("border:3px solid #FFFFFF;");
    OptionBox->setGeometry(32,112,192,32);

    OptionBoxTimer = new QTimer;
    border_color_it=0;
    OptionNum = 5;//选项数
    chooseOption = 0;//选择的选项
    BuyTimes=1;//购买次数
    Currstore_price=40;
    haveBuyTimer=new QTimer(this);
    StoreWinisShow=false;
    setChooseEnable=false;

    connect(OptionBoxTimer, SIGNAL(timeout()), this, SLOT(OptionBoxborderChanged()));
    OptionBoxTimer->start(100);
}
void StoreWidget::OptionBoxborderChanged()
{
    if (border_color_it <= 4)//设置为从0-5，会崩溃，因为当border_color_it=5时，border_color_it++就越界了
        border_color_it++;
    else
        border_color_it = 0;
    //实现选项框边缘颜色的变化（闪烁效果）

    //样式表 setStyleSheet 的组合用法  QString重载了 '+' 号运算符
    OptionBox->setStyleSheet(QString("border:3px solid #") +
                             QString(OptionBoxColor[border_color_it]) + QString(";background-color:transparent;"));
    //OptionBox的背景也为黑色，会遮挡住下面的控件，所以要设置背景为透明
}

void StoreWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    { //向上
        if (chooseOption > 0)
        {
            chooseOption -= 1;
            OptionBox->setGeometry(32, 112 + 48 * chooseOption, 192, 32);
        }
        else
            chooseOption = 0;
    }
    else if (event->key() == Qt::Key_S)
    { //向下
        if (chooseOption <= OptionNum - 2)
        {
            chooseOption += 1;
            OptionBox->setGeometry(32, 112 + 48 * chooseOption, 192, 32);
        }
        else
            chooseOption = OptionNum - 1;
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space)
    {
        if (chooseOption != OptionNum - 1)//不是选择的离开商店选项
        {
            Currstore_price=((BuyTimes) * (BuyTimes) - (BuyTimes) + 2) * 20;
            Nextstore_price = ((BuyTimes +1 ) * (BuyTimes +1 ) - (BuyTimes +1 ) + 2) * 20;

            haveBuyTimer->start(100);//开启购买（计时器开启后不是马上执行，而是在当前函数执行完毕后才开始执行,
                                              //所以，如果在执行计时器后才改变setChooseEnable的值，那么在
                                              //当前函数体内，setChooseEnable的值就还不会改变
            if(!setChooseEnable)//金币不够，则不能购买
            {
                return;
            }

            text1->setText(QString::fromStdWString(L"你若给我 ") +
                           QString::number(Nextstore_price) + QString::fromStdWString(L" 个金币"));
            text2->setText(QString::fromStdWString(L"我就可以帮你"));
            hpOption->setText(QString::fromStdWString(L"提升 ") +
                              QString::number(100 * (BuyTimes +1 )) + QString::fromStdWString(L" 点生命"));
            mpOption->setText(QString::fromStdWString(L"提升 ") +
                              QString::number(20 * (BuyTimes +1 )) + QString::fromStdWString(L" 点魔法"));


            BuyTimes++;

            return;
        }
        else
        {
            this->hide();
            StoreWinisShow=false;
            chooseOption=0;//选项框回到起始位置
            OptionBox->setGeometry(32,112,192,32);
            return;
        }
    }
}
void StoreWidget::showEvent(QShowEvent *)
{
    this->setFocus();//设置焦点在当前窗口
}
