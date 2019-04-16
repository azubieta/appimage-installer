#pragma once

// libary
#include <QStringListModel>

// local
#include <QDialog>
#include "Settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_providersListAddButton_released();

    void on_providersListRemoveButton_released();

    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    Settings settings;

    QStringListModel ocsProvidersListModel;
};
