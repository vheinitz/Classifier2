#ifndef DLGEDITTAG_H
#define DLGEDITTAG_H

#include <QDialog>

namespace Ui {
class DlgEditTag;
}

class DlgEditTag : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgEditTag(QWidget *parent = 0);
    ~DlgEditTag();
    
private slots:
    void on_bCancelDlg_clicked();

    void on_bOk_clicked();

private:
    Ui::DlgEditTag *ui;
};

#endif // DLGEDITTAG_H
