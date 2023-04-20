QT += core gui widgets opengl

major_version = $$QT_MAJOR_VERSION
# 如果qt版本大于5,需要单独引入一些模块
greaterThan(major_version, 5) {
    message(include openglwidgets)
    QT += openglwidgets
}

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

INCLUDEPATH += 3rd_part/tinyobjloader \
                3rd_part/openfbx \
                .

include(gui/gui.pri)
include(function/function.pri)
include(utils/utils.pri)


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../bin

RESOURCES += \
    resources.qrc
