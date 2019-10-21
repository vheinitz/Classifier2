#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "projectdata.h"
#include <cl2/vision/imgdb.h>
#include <QStringListModel>
#include <QVariant>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "objectselector.h"
#include <QPointF>
#include <QRect>
#include <QList>


namespace Ui {
class GUI;
}


class ImageScene;
class ObjectSelectorInterface;

class GUI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();
    
private slots:
    void on_actionNew_Project_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionExit_triggered();
	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();
	void on_lvFiles_clicked(const QModelIndex &index);
    void on_lvClasses_activated(const QModelIndex &index);
	void on_lvClasses_clicked(const QModelIndex &index);
    void on_actionAddImage_triggered();
    void on_actionRemoveImage_triggered();
    void on_actionAddClass_triggered();
    void on_actionRemoveClass_triggered();
	void on_actionEditClass_triggered();

	void on_actionAddTag_triggered();
    void on_actionRemoveTag_triggered();
	void on_actionEditTag_triggered();
	void on_lvTags_clicked(const QModelIndex &index);

    void on_actionSave_Project_triggered();

    void on_actionMark_toggled(bool arg1);
	void updateClasses();
	void updateImage();
	void updateTags();
	void toggleMark(QPointF pos);

	void sceneClicked( QPointF );
	void autosave( );


private:
	void  openProject( QString );

private:
    Ui::GUI *ui;
	QList< QPair<QRect, QGraphicsItem*> > _currentImageMarks;
	ImageScene * _imageScene;
	QImage _currentImage;
	QGraphicsView *_imageView;
	ProjectData _projectData;
	QString _projectFile;
	QString _currentImageId;
	ImageDatabase *_imgDb;
	QStringListModel _images;
	QStandardItemModel _classes;
	QStandardItemModel _tags;
	//QItemSelectionModel _imageTagsSelection;
	QString _lastLoadImagesDir;
	QString _currentClass;
	QString _currentTag;
	ObjectSelector *_objectselector;
	bool _enableAutosave;
};

#endif // GUI_H
