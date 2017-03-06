#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QObject>
#include <QMap>
#include <QPolygon>
#include <QVariant>
#include <QColor>

class RegionInfo
{
public:
	RegionInfo(  ):_classId(-1){}
	RegionInfo( const QPolygon & p, int c ):_polygon(p),_classId(c){}
	QPolygon _polygon;
	int _classId;
};

class ClassInfo
{
public:
	ClassInfo( int id=-1, QString name=QString::null, QColor color=QColor("blue") ):_classId(id),_name(name),_color(color){}
	int _classId;
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
	QMap<int, ClassInfo> _classes;
    
signals:
    
public slots:
	void save( );
	void load( );
	void setRoot( QString r ){_root=r;}
	QString root() const { return _root; }


    
};

#endif // PROJECTDATA_H
