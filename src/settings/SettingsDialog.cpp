// libary
#include <QStringListModel>

// local
#include "ui_SettingsDialog.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ocsProvidersListModel.setStringList(settings.getOCSProviders());
    ui->providersListView->setModel(&ocsProvidersListModel);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_providersListAddButton_released()
{
    if(ocsProvidersListModel.insertRow(ocsProvidersListModel.rowCount())) {
        QModelIndex index = ocsProvidersListModel.index(ocsProvidersListModel.rowCount() - 1, 0);
        ocsProvidersListModel.setData(index, "... new provider url ...");
    }
}

void SettingsDialog::on_providersListRemoveButton_released()
{
    QModelIndexList selection = ui->providersListView->selectionModel()->selectedRows();
    for (const QModelIndex &modelIndex: selection)
    ocsProvidersListModel.removeRow(modelIndex.row());
}

void SettingsDialog::on_buttonBox_accepted()
{
    QStringList ocsProviders = ocsProvidersListModel.stringList();
    settings.setOCSProviders(ocsProviders);
}
