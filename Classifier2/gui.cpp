#include "gui.h"
#include "ui_gui.h"
#include <cl2/gui/imageview.h>
#include <QFileDialog>
#include <cl2/os/persistence.h>
#include <DlgAddClass.h>
#include <dlgedittag.h>
#include <QDateTime>
#include <QTimer>

#include "objectselector.h"


GUI::GUI(QWidget *parent) :
    QMainWindow(parent)
		//,_imageTagsSelection(&_tags),
    ,ui(new Ui::GUI)
{
    ui->setupUi(this);

	_imgDb = new ImageDatabase();

	ui->lvClasses->setModel( &_classes );
	ui->lvTags->setModel( &_tags );
	//ui->lvTags->setSelectionModel(&_imageTagsSelection);
	//_imageTags.setModel( &_tags  );
	ui->lvFiles->setModel( &_images );

	PERSISTENCE_INIT( "heinitz-it.de", "Classifier2" );
    PERSISTENT("_projectFile", &_projectFile, this );
	PERSISTENT("_lastLoadImagesDir", &_lastLoadImagesDir, this );
		
 
	_imageView = new QGraphicsView(this);
	_imageScene = new ImageScene(this);
	_objectselector = new ObjectSelector( _imageScene ); //TODO cross dependencvies
	connect( _imageScene, SIGNAL( clicked(QPointF) ), this, SLOT( sceneClicked(QPointF) ) );
	_imageView->setScene( _imageScene );
	_imageView->setInteractive(true);
	_imageView->adjustSize();
	//_imageScene->setSelector( _objectselector );
	ui->ltImageView->addWidget( _imageView );

	if ( QFileInfo(_projectFile).exists() )
	{
		openProject( _projectFile );
		_projectData.load( );
	}
	updateClasses();

	//_projectData._tags <<"Pos"<<"Neg";
	updateTags();

	_enableAutosave = true;
	QTimer::singleShot(5*60*1000, this, SLOT(autosave()));
}

GUI::~GUI()
{
	_projectData.save();
    delete ui;
}

void GUI::on_actionNew_Project_triggered()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Project Directory"), QFileInfo(_projectFile).canonicalPath() );
	if( !dir.isEmpty() )
	{
		_projectFile = dir + "/" + QFileInfo(dir).fileName() + ".cl2prj";
		{
			QFile (_projectFile).open(QIODevice::WriteOnly);
		}
		openProject(_projectFile);
	}
	else
		return;	
}

void GUI::on_actionOpen_Project_triggered()
{
	QString pf = QFileDialog::getOpenFileName(this, tr("Select Project file"), QFileInfo(_projectFile).canonicalPath(), "*.cl2prj" );
	if( !pf.isEmpty() )
	{
		if (QFileInfo(pf).exists())
			openProject( pf );	
	}
	else
		return;	
}

void  GUI::openProject( QString prjFile )
{
	_projectData._imageRegionClass.clear();
	_projectFile = prjFile;
	_imgDb->setRoot( QFileInfo(_projectFile).canonicalPath() );
	_projectData.setRoot( QFileInfo(_projectFile).canonicalPath() );
	
	_images.setStringList( _imgDb->images() );
	ui->lvFiles->setModel( &_images ) ;
}

void GUI::on_actionExit_triggered()
{
	_projectData.save();
	close();
}

void GUI::on_actionZoom_In_triggered()
{
	if (_currentImage.size().width()>0)
	{
		_imageView->scale(2,2);
	}
}

void GUI::on_actionZoom_Out_triggered()
{
	_imageView->fitInView(_imageScene->sceneRect(), Qt::KeepAspectRatio);
}


void GUI::on_lvFiles_clicked(const QModelIndex &index)
{
	_currentImageId = index.data().toString();
	_currentImage = _imgDb->getImage( QFileInfo(_currentImageId).completeBaseName() );

	_imageScene->clear();
	_imageScene->setImage( QPixmap::fromImage( _currentImage ) );
	_imageView->fitInView(_imageScene->sceneRect(), Qt::KeepAspectRatio);
	//_imageTags.clear();
	updateImage();

	
}

void GUI::on_lvClasses_activated(const QModelIndex &index)
{
	QColor c = index.data(Qt::DecorationRole).value<QColor>();
	_objectselector->setSelectionColor( c );
	_currentClass = index.data(Qt::UserRole+1).toString();
}

void GUI::on_lvClasses_clicked(const QModelIndex &index)
{
	on_lvClasses_activated(index);
}

void GUI::on_lvTags_clicked(const QModelIndex &index)
{
	if (_currentImageId.isEmpty())
		return;
	_currentTag = index.data().toString();
	//_imageTagsSelection.select(index,QItemSelectionModel::ToggleCurrent);
	QString tagId = index.data(Qt::UserRole+1).toString();
	if ( _projectData._imageTags[_currentImageId].contains(tagId) )
	{
		_projectData._imageTags[_currentImageId].removeAll(tagId);
	}
	else
	{
		_projectData._imageTags[_currentImageId].append(tagId);
	}
	
	updateTags();
}

void GUI::on_actionAddImage_triggered()
{
	if ( _projectFile.isEmpty() )
	{
		return;
	}

	QStringList imgFiles = QFileDialog::getOpenFileNames(this, tr("Select Images"), _lastLoadImagesDir, "*.png");
	QStringList added;
	foreach (  QString f, imgFiles )
	{
		added << _imgDb->addImage( f );
	}
	_images.setStringList( _images.stringList() + added );
	ui->lvFiles->setModel( &_images ) ;
}

void GUI::on_actionRemoveImage_triggered()
{
	_projectData._imageRegionClass.remove(_currentImageId);
	_currentImage = QImage();
	_currentImageMarks.clear();
	_imageScene->clear();
	QStringList tmpimages= _images.stringList();
	tmpimages.removeAll( _currentImageId );
	_images.setStringList( tmpimages );
	_imgDb->removeImage( _currentImageId );
	_currentImageId.clear();

}

void GUI::on_actionAddClass_triggered()
{
	DlgAddClass *tmp = new DlgAddClass(this);
	if ( tmp->exec())
	{
		QString id = tmp->id();
		QString name = tmp->name();
		_projectData._classes[id] = ClassInfo(id, name, tmp->_color, tmp->objSize().toInt() );
		//_classes.setStringList( _projectData._classes.values() );
		updateClasses();
	}
}

void GUI::on_actionAddTag_triggered()
{
	DlgEditTag *tmp = new DlgEditTag(this);
	if ( tmp->exec())
	{
		QString id = tmp->id();
		QString name = tmp->name();
		_projectData._tags[id] = TagInfo( id, name, tmp->description() );
		updateTags();
	}
}

void GUI::on_actionEditTag_triggered()
{
	if (_projectData._tags.contains(_currentTag))
	{
		DlgEditTag *tmp = new DlgEditTag(this,
			_projectData._tags[_currentTag]._tagId,
			_projectData._tags[_currentTag]._name,
			_projectData._tags[_currentTag]._description
			);

		if ( tmp->exec())
		{
			QString id = tmp->id();
			QString name = tmp->name();
			QString description = tmp->description();
			_projectData._tags[id] = TagInfo(id, name, description );
			updateTags();
		}
	}
}

void GUI::on_actionEditClass_triggered()
{
	if (_projectData._classes.contains(_currentClass))
	{
		DlgAddClass *tmp = new DlgAddClass(this,
				_projectData._classes[_currentClass]._classId,
				_projectData._classes[_currentClass]._name,
				_projectData._classes[_currentClass]._color,
				_projectData._classes[_currentClass]._size
			);

		if ( tmp->exec())
		{
			QString id = tmp->id();
			QString name = tmp->name();
			_projectData._classes[id] = ClassInfo(id, name, tmp->_color, tmp->objSize().toInt() );
			updateClasses();
		}
	}
}

void GUI::sceneClicked( QPointF pos )
{
	if (!_currentImageId.isEmpty() && !_currentClass.isEmpty())
	{
		toggleMark(pos);
	}
}

void GUI::toggleMark(QPointF pos)
{
	C_LOG("%f, %f",pos.x(),pos.y());
	int size = _projectData._classes[_currentClass]._size;
	QPolygon p(
			QRect(
				pos.x()-size/2,
				pos.y()-size/2,
				size,
				size
				)
			);
	for ( int i=0; i<_currentImageMarks.size(); i++ )
	{
		const QRect &r = _currentImageMarks.at(i).first;
		if ( r.contains( pos.toPoint() ) )
		{
			_imageScene->removeItem( _currentImageMarks.at(i).second );
			_currentImageMarks.removeAt(i);
			break;
		}
	}

	for( int i=0; i < _projectData._imageRegionClass[_currentImageId].size(); i++ )
	{
		if ( _projectData._imageRegionClass[_currentImageId].at(i)._polygon.containsPoint(pos.toPoint(), Qt::OddEvenFill ) )
		{
			_projectData._imageRegionClass[_currentImageId].removeAt(i);
			return;
		}
	}

	QGraphicsItem* tmpgi = _imageScene->addPolygon( p,
				QPen( _projectData._classes[_currentClass]._color ),
				QBrush( _projectData._classes[_currentClass]._color ) );
			tmpgi->setOpacity(.4);
			tmpgi->setSelected(true);
			tmpgi->setActive(true);

	_currentImageMarks.append( QPair<QRect, QGraphicsItem*>( p.boundingRect(), tmpgi) );
	_projectData._imageRegionClass[_currentImageId].append(RegionInfo( p, _currentClass ));

}

void GUI::updateClasses()
{
	_classes.clear();
	foreach ( ClassInfo ci, _projectData._classes)
	{
		QStandardItem *it = new QStandardItem(ci._name);
		it->setData( ci._classId );
		it->setData( ci._color, Qt::DecorationRole );
		_classes.appendRow( it ); 
	}
}

void GUI::updateTags()
{
	_tags.clear();
	foreach ( TagInfo t, _projectData._tags)
	{
		QStandardItem *it = new QStandardItem(t._name);		
		it->setData( t._tagId, Qt::UserRole+1 );
		//it->setSelectable(true);
		it->setCheckable(true);
		if ( _projectData._imageTags[_currentImageId].contains(t._tagId) )
		{
			it->setCheckState(Qt::Checked);
		}
		_tags.appendRow( it ); 

		
	}
}

void GUI::updateImage()
{
	_currentImageMarks.clear();
	if ( _projectData._imageRegionClass.contains(_currentImageId) )
	{
		for( int i=0; i < _projectData._imageRegionClass[_currentImageId].size(); i++ )
		{
			QPolygon tmppol = _projectData._imageRegionClass[_currentImageId].at(i)._polygon;
			if ( _projectData._classes.contains( _projectData._imageRegionClass[_currentImageId].at(i)._classId) )
			{
				QGraphicsItem* tmpgi = _imageScene->addPolygon( tmppol,
					QPen( _projectData._classes[ _projectData._imageRegionClass[_currentImageId].at(i)._classId]._color ),
					QBrush( _projectData._classes[ _projectData._imageRegionClass[_currentImageId].at(i)._classId]._color ) );
				tmpgi->setOpacity(.4);
				tmpgi->setSelected(true);
				tmpgi->setActive(true);
				_currentImageMarks.append( QPair<QRect, QGraphicsItem*>( tmppol.boundingRect(), tmpgi) );
			}
		}
	}
	updateTags();
}

void GUI::on_actionRemoveClass_triggered()
{
	QString id = ui->lvClasses->currentIndex().data(Qt::UserRole+1).toString(); //TODO userrol1+1 = clasid
	int r = ui->lvClasses->currentIndex().row();
	_classes.removeRow(r);
	_projectData._classes.remove(id);
	_currentClass.clear();

}

void GUI::on_actionRemoveTag_triggered()
{
	QString id = ui->lvTags->currentIndex().data(Qt::UserRole+1).toString(); 
	int r = ui->lvTags->currentIndex().row();
	_tags.removeRow(r);
	_projectData._tags.remove(id);
	_currentClass.clear();
}

void GUI::autosave( )
{
	if (this->_enableAutosave)
	{
		this->_projectData.save( QString::number( QDateTime::currentMSecsSinceEpoch()/1000 ) );
		QTimer::singleShot(5*60*1000, this, SLOT(autosave()));
	}
}

void GUI::on_actionSave_Project_triggered()
{
	_projectData.save();
}

void GUI::on_actionMark_toggled(bool m)
{
	_imageScene->setMarkingActive( m );
}
