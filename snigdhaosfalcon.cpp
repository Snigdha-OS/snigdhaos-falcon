#include "snigdhaosfalcon.h"
#include "ui_snigdhaosfalcon.h"
#include <QMessageBox>

SnigdhaOSFalcon::SnigdhaOSFalcon(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SnigdhaOSFalcon)
{
    ui->setupUi(this);
    updateCheckBoxes();
}

SnigdhaOSFalcon::~SnigdhaOSFalcon()
{
    delete ui;
}

void SnigdhaOSFalcon::updatePackageList() {

}
