#include "camera.h"

#include <QtWidgets>
#include <DApplication>
#include <DWidgetUtil>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-camera");
    app.setApplicationVersion("1.0");
    app.setProductIcon(QIcon(":/images/logo.svg"));
    app.setProductName(QObject::tr("Deepin Camera"));
    app.setApplicationDescription(QObject::tr("Camera application for Deepin Desktop Environment"));
    app.setApplicationLicense("GPL-3.0");

    Camera camera;
    camera.setMinimumSize(800, 600);
    camera.show();

    Dtk::Widget::moveToCenter(&camera);

    return app.exec();
}
