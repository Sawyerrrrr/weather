#include "mainwindow.h"
#include "weathercity.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(width(),height());
    setWindowTitle("天气预报");
    mExitMenu = new QMenu(this);
    mExitAct = new QAction();
    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon(":/res/close.png"));
    mExitMenu->addAction(mExitAct);

    connect(mExitAct,&QAction::triggered,this,[=]{
        qApp->exit(0);    //#define qApp (static_cast<QApplication *>(QCoreApplication::instance())) 指向应用程序对象的指针
    });


    mWeekList<<ui->lbWeek0<<ui->lbWeek1<<ui->lbWeek2<<ui->lbWeek3<<ui->lbWeek4<<ui->lbWeek5;
    mDateList<<ui->lbDate0<<ui->lbDate1<<ui->lbDate2<<ui->lbDate3<<ui->lbDate4<<ui->lbDate5;
    mTypeList<<ui->lbType0<<ui->lbType1<<ui->lbType2<<ui->lbType3<<ui->lbType4<<ui->lbType5;
    mTypeIconList<<ui->lbTypeIcon0<<ui->lbTypeIcon1<<ui->lbTypeIcon2
                <<ui->lbTypeIcon3<<ui->lbTypeIcon4<<ui->lbTypeIcon5;

    mAqiList<<ui->lbAqi0<<ui->lbAqi1<<ui->lbAqi2<<ui->lbAqi3<<ui->lbAqi4<<ui->lbAqi5;
    mFxList<<ui->lbFx0<<ui->lbFx1<<ui->lbFx2<<ui->lbFx3<<ui->lbFx4<<ui->lbFx5;
    mFlList<<ui->lbFl0<<ui->lbFl1<<ui->lbFl2<<ui->lbFl3<<ui->lbFl4<<ui->lbFl5;

    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大到暴雨",":/res/type/DaDaoBaoYu.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");

    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");



    mManager=new QNetworkAccessManager(this);
    connect(mManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);
    //getWeatherInfo("101010100");
    getWeatherInfo("西安");



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::contextMenuEvent(QContextMenuEvent *event)  //右键菜单事件,重写父类的虚函数。父类中默认忽略右键菜单事件
{
    mExitMenu->exec(QCursor::pos());
    event->accept();  //事件已处理，不需要向上传递
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mOffset= event->globalPos()-this->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
    //move的原点是父窗口的左上角，  如果没有父窗口，则桌面即为父窗口 x,y, x->


}

void MainWindow::getWeatherInfo(QString cityName)
{
    weathercity wc;
    QString cityCode=wc.getCityCode(cityName);
    if(cityCode.isEmpty()){
        QMessageBox::warning(this,"错误","输入有误！",QMessageBox::Ok);
        return;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/"+cityCode);
    mManager->get(QNetworkRequest(url));
}

void MainWindow::parseJson(QByteArray byteArray)
{
    QJsonParseError err;
    QJsonDocument doc= QJsonDocument::fromJson(byteArray,&err);
    if(err.error!=QJsonParseError::NoError){
        return;
    }
    QJsonObject rootObj=doc.object();
    mToday.date=rootObj.value("date").toString();
    mToday.city=rootObj.value("cityInfo").toObject().value("city").toString();

    QJsonObject objData=rootObj.value("data").toObject();
    QJsonObject objYesterday=objData.value("yesterday").toObject();
    mDay[0].week=objYesterday.value("week").toString();
    mDay[0].date=objYesterday.value("ymd").toString();
    mDay[0].type=objYesterday.value("type").toString();

    QString s;
    s=objYesterday.value("high").toString().split(" ").at(1);
    s=s.left(s.length()-1);
    mDay[0].high=s.toInt();

    s=objYesterday.value("low").toString().split(" ").at(1);
    s=s.left(s.length()-1);
    mDay[0].low=s.toInt();

    mDay[0].fx=objYesterday.value("fx").toString();
    mDay[0].fl=objYesterday.value("fl").toString();

    mDay[0].aqi=objYesterday.value("aqi").toInt();

    QJsonArray fcastArr=objData.value("forecast").toArray();   //mDay[0]存放昨天的数据，mDay[1]存放今天的数据

    for(int i=0;i<5;++i){
        QJsonObject objForecast=fcastArr[i].toObject();
        mDay[i+1].week=objForecast.value("week").toString();
        mDay[i+1].date=objForecast.value("ymd").toString();

        mDay[i+1].type=objForecast.value("type").toString();

        QString s;
        s=objForecast.value("high").toString().split(" ").at(1);  //高温 10°C
        s=s.left(s.length()-1);  //去掉右边最后一个符号
        mDay[i+1].high=s.toInt(); //转int 需要全数字

        s=objForecast.value("low").toString().split(" ").at(1);
        s=s.left(s.length()-1);
        mDay[i+1].low=s.toInt();

        mDay[i+1].fx=objForecast.value("fx").toString();
        mDay[i+1].fl=objForecast.value("fl").toString();

        mDay[i+1].aqi=objForecast.value("aqi").toInt();
    }
    mToday.ganmao=objData.value("ganmao").toString();
    mToday.wendu=objData.value("wendu").toString();
    mToday.shidu=objData.value("shidu").toString();
    mToday.pm25=objData.value("pm25").toDouble();
    mToday.quality=objData.value("quality").toString();

    mToday.type=mDay[1].type;
    mToday.fx=mDay[1].fx;
    mToday.fl=mDay[1].fl;
    mToday.high=mDay[1].high;
    mToday.low=mDay[1].low;

    updateUi();


}

void MainWindow::updateUi()
{
    ui->lbDate0->setText(QDateTime::fromString(mToday.date,"yyyyMMdd").toString("yyyy/MM/dd")+" "+mDay[1].week);
    //返回datetime然后tostirng  MM.dd.yyyy
    ui->lbCity->setText(mToday.city);

    ui->lbTemp->setText(QString("当前温度: ")+mToday.wendu+" °C");
    ui->lbType->setText(mToday.type);
    ui->lbLowHigh->setText(QString("今日温度: ")+QString::number(mToday.low)+" ~ "+QString::number(mToday.high)+" °C");
    ui->lbGanMao->setText("感冒指数："+mToday.ganmao);
    ui->lbWindFx->setText(QString("风向: ")+mToday.fx);
    ui->lbWindFl->setText(QString("风力: ")+mToday.fl);
    ui->lbPm25->setText(QString("pm2.5: ")+QString::number(mToday.pm25));
    ui->lbShidu->setText(QString("湿度: ")+mToday.shidu);
    ui->lbQuality->setText(QString("空气质量: ")+mToday.quality);

    for(int i=0;i<6;++i){
        mWeekList[i]->setText("周"+mDay[i].week.right(1));
        ui->lbWeek0->setText("昨天");
        ui->lbWeek1->setText("今天");
        ui->lbWeek2->setText("明天");

        QStringList ymdList=mDay[i].date.split("-");
        mDateList[i]->setText(ymdList[1]+"/"+ymdList[2]);

        mTypeList[i]->setText(mDay[i].type);
        mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);

        if(mDay[i].aqi>0 && mDay[i].aqi<=50){
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color:rgb(121,184,0);");

        }else if(mDay[i].aqi>50 && mDay[i].aqi<=100){
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color:rgb(255,187,23);");

        }else if(mDay[i].aqi>100 && mDay[i].aqi<=150){
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color:rgb(255,87,97);");

        }else if(mDay[i].aqi>150 && mDay[i].aqi<=200){
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color:rgb(235,17,27);");

        }else if(mDay[i].aqi>200 && mDay[i].aqi<=250){
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color:rgb(170,0,0);");

        }else if(mDay[i].aqi>0 && mDay[i].aqi<=50){
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color:rgb(110,0,0);");

        }
          mFxList[i]->setText(mDay[i].fx);
          mFlList[i]->setText(mDay[i].fl);

    }

}

void MainWindow::onReplied(QNetworkReply *reply)
{
    int statusCode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error()!=QNetworkReply::NoError || statusCode!=200){
         qDebug()<<reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"weather","请求数据失败",QMessageBox::Ok);
    }else{
       QByteArray byteArray= reply->readAll();
       parseJson(byteArray);
    reply->deleteLater();
}
}

void MainWindow::on_pushButton_clicked()
{
    QString cityName=ui->lineEdit->text();
    getWeatherInfo(cityName);
}
