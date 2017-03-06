#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "projectdata.h"
#include <cl2/vision/imgdb.h>
#include <QStringListModel>
#include <QVariant>
#include <QStandardItemModel>


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
	void accepted(QPolygon);
    void on_actionNew_Project_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionExit_triggered();
	void on_lvFiles_clicked(const QModelIndex &index);
    void on_lvClasses_activated(const QModelIndex &index);
	void on_lvClasses_clicked(const QModelIndex &index);
    void on_actionAddImage_triggered();
    void on_actionRemoveImage_triggered();
    void on_actionAddClass_triggered();
    void on_actionRemoveClass_triggered();

    void on_actionSave_Project_triggered();

    void on_actionMark_toggled(bool arg1);
	void updateClasses();

private:
	void  openProject( QString );

private:
    Ui::GUI *ui;
	ImageScene * _imageScene;
	ProjectData _projectData;
	QString _projectFile;
	ImageDatabase _imgDb;
	QStringListModel _images;
	QStandardItemModel _classes;
	QString _lastLoadImagesDir;
	int _currentClass;
	ObjectSelectorInterface *_objectselector;
};

#endif // GUI_H
