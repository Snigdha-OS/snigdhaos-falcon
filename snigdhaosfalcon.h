#ifndef SNIGDHAOSFALCON_H
#define SNIGDHAOSFALCON_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class SnigdhaOSFalcon;
}
class QCheckBox;
class QAbstractButton;
QT_END_NAMESPACE

class SnigdhaOSFalcon : public QMainWindow
{
    Q_OBJECT

public:
    SnigdhaOSFalcon(QWidget *parent = nullptr);
    ~SnigdhaOSFalcon();

private:
    QStringList installed_packages;
    QProcess installer;
    bool allowUnlock;

    QList<QCheckBox*> getCheckboxes();
    void updateCheckBoxes();
    void lockCheckBoxes(bool locked);
    void updatePackageList();

private slots:
    void on_installerFinished(int, QProcess::ExitStatus);
    void on_applyButtons_clicked(QAbstractButton* button);

private:
    Ui::SnigdhaOSFalcon *ui;
};
#endif // SNIGDHAOSFALCON_H
