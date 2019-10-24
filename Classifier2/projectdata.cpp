#include "projectdata.h"
#include <QStringList>
#include <cl2/os/fstools.h>

ProjectData::ProjectData(QObject *parent) :
    QObject(parent)
{
}


void ProjectData::save( QString suffix )
{
	QStringList data;
	foreach ( QString fn, _imageRegionClass.keys() )
	{
		foreach( RegionInfo ri, _imageRegionClass[fn] )
		{
			if ( _classes.contains( ri._classId) )
			{
				QStringList pol;
				foreach ( QPoint p, ri._polygon.toList() )
					pol<< QString("%1:%2").arg(p.x()).arg(p.y());

				data << QString("%1;%2;%3,").arg(fn).arg( ri._classId ).arg(pol.join(","));
			}
		}
	}
	FSTools::toFile( data, _root+QString("/regions%1").arg(suffix) );

	QStringList imgTagData;
	foreach ( QString fn, _imageTags.keys() )
	{
		foreach( TagInfo ri, _imageTags[fn] )
		{
			if ( _tags.contains( ri._tagId) )
			{				
				imgTagData << QString("%1;%2").arg(fn).arg( ri._tagId );
			}
		}
	}
	FSTools::toFile( imgTagData, _root+QString("/imagetags%1").arg(suffix) );


	QStringList cldata;
	foreach ( QString cid, _classes.keys() )
	{
		cldata << QString("%1;%2;%3;%4")
			.arg(cid)
			.arg( _classes[cid]._name )
			.arg( _classes[cid]._color.name() )
			.arg( _classes[cid]._size );
	}
	FSTools::toFile( cldata, _root+QString("/classes%1").arg(suffix) );

	QStringList tagData;
	foreach ( QString tid, _tags.keys() )
	{
		tagData << QString("%1;%2;%3")
			.arg(tid)
			.arg( _tags[tid]._name )
			.arg( QString(_tags[tid]._description.toAscii().toBase64()) );
	}
	FSTools::toFile( tagData, _root+QString("/tags%1").arg(suffix) );

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
		ri._classId = dentries.at(1);
		QVector<QPoint> vpoints;
		foreach( QString p, dentries.at(2).split(",") )
		{
			if ( !p.isEmpty())
			{
				vpoints << QPoint( p.section(":",0,0).toInt(), p.section(":",1,1).toInt() );
			}
		}
		ri._polygon = QPolygon(vpoints);
		_imageRegionClass[dentries.at(0)].append(ri);
	}

	data = FSTools::fromFile( _root+"/classes" );
	_classes.clear();
	foreach ( QString entry, data )
	{
		QStringList dentries = entry.split(";");
		if (dentries.size() < 4) continue;
		ClassInfo classInfo;
		classInfo._classId = dentries.at(0);
		
		bool ok=0;
		QString col = dentries.at(2).section("#",1);
		int r = col.mid( 0,2).toInt(&ok,16);
		int g = col.mid( 2,2).toInt(&ok,16);
		int b = col.mid( 4,2).toInt(&ok,16);
		classInfo._color = QColor::fromRgb( r,g,b );
		
		classInfo._name =dentries.at(1);
		classInfo._size =dentries.at(3).toInt();
		_classes[dentries.at(0)] = classInfo;
	}

	data = FSTools::fromFile( _root+"/tags" );
	_tags.clear();
	foreach ( QString entry, data )
	{
		QStringList dentries = entry.split(";");
		if (dentries.size() < 3) continue;
		TagInfo tagInfo;
		tagInfo._tagId = dentries.at(0);
		tagInfo._name = dentries.at(1);
		tagInfo._description = QByteArray::fromBase64(dentries.at(2).toAscii());

		_tags[tagInfo._tagId] = tagInfo;
	}

	data = FSTools::fromFile( _root+"/imagetags" );
	_imageTags.clear();
	foreach ( QString entry, data )
	{
		QStringList dentries = entry.split(";");
		if (dentries.size() < 2) continue;
		_imageTags[dentries.at(0)].append( dentries.at(1) );
	}

}