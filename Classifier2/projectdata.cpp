#include "projectdata.h"
#include <QStringList>
#include <cl2/os/fstools.h>

ProjectData::ProjectData(QObject *parent) :
    QObject(parent)
{
}


void ProjectData::save( )
{
	QStringList data;
	foreach ( QString fn, _imageRegionClass.keys() )
	{
		foreach( RegionInfo ri, _imageRegionClass[fn] )
		{
			QStringList pol;
			foreach ( QPoint p, ri._polygon.toList() )
				pol<< QString("%1:%2").arg(p.x()).arg(p.y());

			data << QString("%1; %2; %3").arg(fn).arg( ri._classId ).arg(pol.join(","));
		}
	}

	FSTools::toFile( data, _root+"/regions" );


	QStringList cldata;
	foreach ( int cid, _classes.keys() )
	{
		cldata << QString("%1; %2; %3").arg(cid).arg( _classes[cid]._name ).arg( _classes[cid]._color.name() );
	}

	FSTools::toFile( cldata, _root+"/classes" );
}

void ProjectData::load( )
{
	QStringList data = FSTools::fromFile( _root+"/regions" );
	_imageRegionClass.clear();
	foreach ( QString entry, data )
	{
		QStringList dentries = entry.split(";");
		if (dentries.size() < 3) continue;
		RegionInfo ri;
		ri._classId = dentries.at(1).toInt();
		QVector<QPoint> vpoints;
		foreach( QString p, dentries.at(2).split(",") )
		{
			vpoints << QPoint( p.section(":",0,0).toInt(), p.section(":",1,1).toInt() );			
		}
		ri._polygon = QPolygon(vpoints);
		_imageRegionClass[dentries.at(0)].append(ri);
	}

	data = FSTools::fromFile( _root+"/classes" );
	_classes.clear();
	foreach ( QString entry, data )
	{
		QStringList dentries = entry.split(";");
		if (dentries.size() < 3) continue;
		ClassInfo classInfo;
		classInfo._classId = dentries.at(0).toInt();
		//if ( QColor::isValidColor( dentries.at(2) ) )
		{
			bool ok=0;
			QString col = dentries.at(2).section("#",1);
			int r = col.mid( 0,2).toInt(&ok,16);
			int g = col.mid( 2,2).toInt(&ok,16);
			int b = col.mid( 4,2).toInt(&ok,16);
			classInfo._color = QColor::fromRgb( r,g,b );
		}
		classInfo._name =dentries.at(1);
		_classes[dentries.at(0).toInt()] = classInfo;
	}

}