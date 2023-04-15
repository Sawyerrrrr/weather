#ifndef WEATHERDATA_H
#define WEATHERDATA_H
#include<QString>
class Today{
public:
    Today(){
        date="2022-3-15";
        city="西安";
        ganmao="感冒指数";
        shidu="0%";
        quality="无数据";
        type="多云";
        fl="2级";
        fx="南风";
        high=30;
        low=18;
    }
    QString date;
    QString city;
    QString ganmao;
    QString wendu;
    QString shidu;
    int pm25;
    QString quality;
    QString type;
    QString fx;
    QString fl;
    int high;
    int low;


};


class Day{
public:
    Day(){
    date="2022-3-15";
    week="周五";
    type="多云";
    fl="2级";
    fx="南风";
    high=0;
    low=0;
    aqi=0;
}
    QString date;
    QString week;
    QString type;
    QString fx;
    QString fl;
    int high;
    int low;
    int aqi;


};











#endif // WEATHERDATA_H
