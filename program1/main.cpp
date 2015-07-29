#include <QApplication>
#include <QSurfaceFormat>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);

    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget window;
    window.show();
    return app.exec();
}
