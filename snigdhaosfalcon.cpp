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

void SnigdhaOSFalcon::updateCheckBoxes() {
    updatePackageList();
    for (auto element : getCheckboxes()) {
        auto string = element->property("packageName").toString();
        element->setChecked(installed_packages.contains(string));
    }
}

void SnigdhaOSFalcon::lockCheckBoxes(bool locked) {
    for (auto element : getCheckboxes()) {
        element->setAttribute(Qt::WA_TransparentForMouseEvents, locked);
        element->setFocusPolicy(locked ? Qt::NoFocus : Qt::StrongFocus);
    }
    this->ui->applyButtons->setAttribute(Qt::WA_TransparentForMouseEvents, locked);
    this->ui->applyButtons->setFocusPolicy(locked ? Qt::NoFocus : Qt::StrongFocus);
}

void SnigdhaOSFalcon::on_installerFinished(int, QProcess::ExitStatus) {
    if (!allowUnlock){
        return;
    }
    updateCheckBoxes();
    lockCheckBoxes(false);
}

void SnigdhaOSFalcon::on_applyButtons_clicked(QAbstractButton* button) {
    if (installer.state() != installer.NotRunning){
        return;
    }
    if (ui->applyButtons->buttonRole(button) == ui->applyButtons->ApplyRole) {
        updatePackageList();
        QStringList removed_packages;
        QStringList added_packages;

        for (auto element : getCheckboxes()) {
            auto packagename = element->property("packageName").toString();
            auto checked = element->isChecked();

            if (installed_packages.contains(packagename)){
                if (!checked) {
                    removed_packages.append(std::move(packagename));
                }
                else {
                    if (checked) {
                        added_packages.append(std::move(packagename));
                    }
                }
            }
        }

        if (added_packages.empty() && removed_packages.empty()){
            QMessageBox::warning(this, "Snigdha OS Falcon", "The current packages/config is already applied on your machine!");
            return;
        }
        allowUnlock = false;
        lockCheckBoxes(true);
        QString cmd;
        if (!added_packages.empty()){
            cmd.append("pacman -S --noconfirm " + added_packages.join(" ") + ";");
        }
        if (!removed_packages.empty()){
            cmd.append("pacman -Rns --noconfirm " + removed_packages.join(" ") + ";");
        }

        cmd += "read -p 'Press Enter↵ to Exit!'";

        auto paramlist = QStringList();
        paramlist << "-s" << "pkexec /usr/share/snigdhaos/scripts/snigdhaos-falcon/shellrunner.sh";
        paramlist << cmd;
        allowUnlock = true;
        installer.start("/usr/lib/snigdhaos/launch-terminal",paramlist);
    }
    else {
        updateCheckBoxes();
    }
}
