// library
#include <QApplication>

// local
#include "SettingsDialog.h"

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    QApplication::setApplicationDisplayName(QApplication::tr("AppImage User Tool Settings"));

    SettingsDialog dialog;

    dialog.setWindowTitle(QApplication::applicationDisplayName());
    dialog.show();

    return QApplication::exec();
}
