#include "dlgaddclass.h"
#include "ui_dlgaddclass.h"
#include <QColorDialog>

DlgAddClass::DlgAddClass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddClass)
{
    ui->setupUi(this);
}

DlgAddClass::~DlgAddClass()
{
    delete ui;
}

void DlgAddClass::on_bCancel_clicked()
{
	reject();
    close();
}

void DlgAddClass::on_bAdd_clicked()
{
	accept();
    close();
}

int DlgAddClass::id() const
{
	return ui->eClass->text().toInt();
}

QString DlgAddClass::name() const
{
	return ui->eClassName->text();
}

void DlgAddClass::on_bSelectcolor_clicked()
{
    _color = QColorDialog::getColor();
	ui->fColor->setStyleSheet( QString("background-color: rgb(%1,%2,%3);").arg(_color.red()).arg(_color.green()).arg(_color.blue()) );
}
