#include "gui.h"
#include "ui_gui.h"
#include <cl2/ui/imageview.h>
#include <QFileDialog>
#include <cl2/os/persistence.h>
#include <DlgAddClass.h>
#include "objectselector.h"


GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI),
	_currentClass(-1)
{
    ui->setupUi(this);

	ui->lvClasses->setModel( &_classes );
	ui->lvFiles->setModel( &_images );

	PERSISTENCE_INIT( "heinitz-it.de", "Classifier2" );
    PERSISTENT("_projectFile", &_projectFile, this );
	PERSISTENT("_lastLoadImagesDir", &_lastLoadImagesDir, this );
		
 
	QGraphicsView *imageView = new QGraphicsView(this);
	_imageScene = new ImageScene(this);
	_objectselector = new ObjectSelector( _imageScene ); //TODO cross dependencvies
	connect( _objectselector, SIGNAL( accepted(QPolygon) ), this, SLOT( accepted(QPolygon) ) );
	imageView->setScene( _imageScene );
	_imageScene->setSelector( _objectselector );
	ui->ltImageView->addWidget( imageView );

	if ( QFileInfo(_projectFile).exists() )
	{
		openProject( _projectFile );
		_projectData.load( );
	}
	updateClasses();
}

GUI::~GUI()
{
    delete ui;
}

void GUI::accepted(QPolygon p)
{
	if (_currentClass>0)
		_projectData._imageRegionClass[ ui->lvFiles->currentIndex().data().toString() ].append( RegionInfo (p, _currentClass) );
}

void GUI::on_actionNew_Project_triggered()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Project Directory"), QFileInfo(_projectFile).canonicalPath() );
	if( !dir.isEmpty() )
	{
		_projectFile = dir + "/" + QFileInfo(dir).fileName() + ".cl2prj";
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
	_imgDb.setRoot( QFileInfo(_projectFile).canonicalPath() );
	_projectData.setRoot( QFileInfo(_projectFile).canonicalPath() );
	
	_images.setStringList( _imgDb.images() );
	ui->lvFiles->setModel( &_images ) ;
}

void GUI::on_actionExit_triggered()
{
	_projectData.save();
	close();
}


void GUI::on_lvFiles_clicked(const QModelIndex &index)
{
	QImage img = _imgDb.getPreview( QFileInfo(index.data().toString()).completeBaseName() );

	_imageScene->setImage( QPixmap::fromImage( img ) );

	QString fn = ui->lvFiles->currentIndex().data().toString();
	if ( _projectData._imageRegionClass.contains(fn) )
	{
		for( int i=0; i < _projectData._imageRegionClass[fn].size(); i++ )
		{
			QGraphicsItem* p = _imageScene->addPolygon( _projectData._imageRegionClass[fn].at(i)._polygon,
				QPen( _projectData._classes[ _projectData._imageRegionClass[fn].at(i)._classId]._color ),
				QBrush( _projectData._classes[ _projectData._imageRegionClass[fn].at(i)._classId]._color ) );
			p->setOpacity(.4);
			p->setSelected(true);
			p->setActive(true);
		}
	}

	
}

void GUI::on_lvClasses_activated(const QModelIndex &index)
{
	QColor c = index.data(Qt::DecorationRole).value<QColor>();
	_objectselector->setSelectionColor( c );
	_currentClass = index.data(Qt::UserRole+1).toInt();
}

void GUI::on_lvClasses_clicked(const QModelIndex &index)
{
	on_lvClasses_activated(index);
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
		added << _imgDb.addImage( f );
	}
	_images.setStringList( _images.stringList() + added );
	ui->lvFiles->setModel( &_images ) ;
}

void GUI::on_actionRemoveImage_triggered()
{

}

void GUI::on_actionAddClass_triggered()
{
	DlgAddClass *tmp = new DlgAddClass(this);
	if ( tmp->exec())
	{
		int id = tmp->id();
		QString name = tmp->name();
		_projectData._classes[id] = ClassInfo(id, name, tmp->_color );
		//_classes.setStringList( _projectData._classes.values() );
		updateClasses();
	}
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

void GUI::on_actionRemoveClass_triggered()
{
	int id = ui->lvClasses->currentIndex().data(Qt::UserRole+1).toInt(); //TODO userrol1+1 = clasid
	int r = ui->lvClasses->currentIndex().row();
	_classes.removeRow(r);
	_projectData._classes.remove(id);

}

void GUI::on_actionSave_Project_triggered()
{
	_projectData.save();
}

void GUI::on_actionMark_toggled(bool m)
{
	_imageScene->setMarkingActive( m );
}
