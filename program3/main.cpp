#include <QApplication>

#include "glwidget.h"

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(texture);

    QApplication a(argc, argv);

    QSurfaceFormat format; 
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget glwidget;
    glwidget.show();

    return a.exec();
}
