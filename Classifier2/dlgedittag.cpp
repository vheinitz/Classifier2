#include "dlgedittag.h"
#include "ui_dlgedittag.h"

DlgEditTag::DlgEditTag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgEditTag)
{
    ui->setupUi(this);
}

DlgEditTag::~DlgEditTag()
{
    delete ui;
}

void DlgEditTag::on_bCancelDlg_clicked()
{

}

void DlgEditTag::on_bOk_clicked()
{

}
