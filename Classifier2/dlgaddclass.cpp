#include "dlgaddclass.h"
#include "ui_dlgaddclass.h"
#include <QColorDialog>
#include <QUuid>

DlgAddClass::DlgAddClass(QWidget *parent, QString id, QString name, QColor color, int size) :
    QDialog(parent),
    ui(new Ui::DlgAddClass)
{
    ui->setupUi(this);
	if ( color != QColor() )
	{
		ui->fColor->setStyleSheet( QString("background-color: %1;").arg(color.name()));
		_color = color;
	}
	else
	{
		ui->fColor->setStyleSheet( QString("background-color: rgb(0,0,0);"));
		_color = QColor(Qt::black);
	}

	if (size>0)
	{
		ui->eSize->setText( QString::number(size) );
	}

	if (!id.isEmpty())
	{
		ui->eClass->setText( id );		
		ui->bAdd->setText(tr("Save"));
	}
	else
	{
		ui->eClass->setText( QUuid::createUuid().toString() );
	}

	if (!name.isEmpty())
	{
		ui->eClassName->setText( name );
	}
	
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

QString DlgAddClass::id() const
{
	return ui->eClass->text();
}

QString DlgAddClass::name() const
{
	return ui->eClassName->text();
}

QString DlgAddClass::objSize() const
{
	return ui->eSize->text();
}

void DlgAddClass::on_bSelectcolor_clicked()
{
    _color = QColorDialog::getColor();
	ui->fColor->setStyleSheet( QString("background-color: rgb(%1,%2,%3);").arg(_color.red()).arg(_color.green()).arg(_color.blue()) );
}
