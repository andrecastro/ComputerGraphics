#include <QApplication>
#include "ui_mainwindow.h"

#include "glwidget.h"

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(textures);

    QApplication a(argc, argv);

    QSurfaceFormat format; 
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QMainWindow *window = new QMainWindow;
    Ui::MainWindow ui;
    ui.setupUi(window);

    window->show();

    return a.exec();
}
