#include "snigdhaosfalcon.h"
#include "ui_snigdhaosfalcon.h"
#include <QMessageBox>

SnigdhaOSFalcon::SnigdhaOSFalcon(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SnigdhaOSFalcon)
{
    ui->setupUi(this);
    updateCheckBoxes();
    connect(&installer, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_installerFinished(int,QProcess::ExitStatus)));
}

SnigdhaOSFalcon::~SnigdhaOSFalcon()
{
    delete ui;
}

QList<QCheckBox*> SnigdhaOSFalcon::getCheckboxes() {
    return this->ui->tabWidget->findChildren<QCheckBox*>();
}

void SnigdhaOSFalcon::updatePackageList() {
    QStringList packages;
    for (auto element : getCheckboxes()) {
        auto string = element->property("packageName").toString();
        if (string.isEmpty()) {
            std::runtime_error(("Empty/Non existent package name detected for " + element->objectName()).toStdString());
        }
        packages.append(std::move(string));
    }

    QProcess proc;
    proc.start("pacman -Qq " + packages.join(" "));
    proc.waitForFinished();
    QString installed = proc.readAllStandardOutput();
    installed_packages = installed.split("\n");
}
