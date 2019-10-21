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
	explicit DlgEditTag(QWidget *parent = 0, QString id=QString::null, QString name=QString::null, QString description=QString::null);
    ~DlgEditTag();
	QString id() const;
	QString name() const;
	QString description() const;
    
private slots:
    void on_bCancelDlg_clicked();

    void on_bOk_clicked();

private:
    Ui::DlgEditTag *ui;
};

#endif // DLGEDITTAG_H
