#include "dlgedittag.h"
#include "ui_dlgedittag.h"
#include <QUuid>

DlgEditTag::DlgEditTag( QWidget *parent, QString id, QString name, QString description ) :
    QDialog(parent),
    ui(new Ui::DlgEditTag)
{
    ui->setupUi(this);

	if (!id.isEmpty())
	{
		ui->eTagId->setText( id );		
		ui->bOk->setText(tr("Save"));
	}
	else
	{
		ui->eTagId->setText( QUuid::createUuid().toString() );
	}

	if (!name.isEmpty())
	{
		ui->eTag->setText( name );
	}

	if (!description.isEmpty())
	{
		ui->tDescription->setPlainText( description );
	}
}

DlgEditTag::~DlgEditTag()
{
    delete ui;
}

void DlgEditTag::on_bCancelDlg_clicked()
{
	reject();
    close();
}

void DlgEditTag::on_bOk_clicked()
{
	accept();
    close();
}

QString DlgEditTag::id() const
{
	return ui->eTagId->text();
}

QString DlgEditTag::name() const
{
	return ui->eTag->text();
}

QString DlgEditTag::description() const
{
	return ui->tDescription->toPlainText();
}