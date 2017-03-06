#ifndef DLGADDCLASS_H
#define DLGADDCLASS_H

#include <QDialog>

namespace Ui {
class DlgAddClass;
}

class DlgAddClass : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAddClass(QWidget *parent = 0);
    ~DlgAddClass();
	int id() const;
	QString name() const;
	QColor _color;
    
private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

    void on_bSelectcolor_clicked();

private:
    Ui::DlgAddClass *ui;
};

#endif // DLGADDCLASS_H
