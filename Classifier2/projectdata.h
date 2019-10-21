#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include <QMap>
#include <QPolygon>
#include <QVariant>
#include <QColor>
#include <QGraphicsItem>

class RegionInfo
{
public:
	RegionInfo(  ):_classId(-1){}
	RegionInfo( const QPolygon & p, QString c ):_polygon(p),_classId(c){}
	QPolygon _polygon;
	QString _classId;
};

class ClassInfo
{
public:
	ClassInfo( QString id=QString::null, QString name=QString::null, QColor color=QColor("blue"), int s=64 ):
	  _classId(id),_name(name),_color(color),_size(s){}
	QString _classId;
	int _size;
	QString _name;
	QColor _color;
};

class ProjectData : public QObject
{
    Q_OBJECT
public:
    explicit ProjectData(QObject *parent = 0);
    QString _name;
	QString _root;
    QMap<QString, QList< RegionInfo > > _imageRegionClass;
	QMap<QString, QStringList > _imageTags;
	QMap<QString, ClassInfo> _classes;
	QStringList _tags;
    
signals:
    
public slots:
	void save( QString suffix = QString::null);
	void load( );
	void setRoot( QString r ){_root=r;}
	QString root() const { return _root; }


    
};

#endif // PROJECTDATA_H
