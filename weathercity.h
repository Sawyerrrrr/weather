#ifndef WEATHERCITY_H
#define WEATHERCITY_H
#include<QMap>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
class weathercity{
public:
     QString getCityCode(QString cityname){
        if(mCityMap.isEmpty()){
            initCityMap();
        }
        QMap<QString,QString>::iterator it =mCityMap.find(cityname);
        if(it==mCityMap.end()){
            it=mCityMap.find(cityname+"市");
        }
        if(it!=mCityMap.end()){
            return it.value();
        }
        return "";
    }
private:
     QMap<QString,QString>mCityMap;

     void initCityMap(){
        QString filePath=":/res/citycode.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly |QIODevice::Text);
        QByteArray json=file.readAll();
        file.close();

        QJsonParseError err;
        QJsonDocument doc=QJsonDocument::fromJson(json,&err);
        if(err.error!=QJsonParseError::NoError){
            return;
        }

        if(!doc.isArray()){
            return;
        }

        QJsonArray cities=doc.array();
        for(int i=0;i<cities.size();++i){
            QString city=cities[i].toObject().value("city_name").toString();
            QString code=cities[i].toObject().value("city_code").toString();
            if(code.size()>0){
            mCityMap.insert(city,code);
            }

        }

    }
};




#endif // WEATHERCITY_H
