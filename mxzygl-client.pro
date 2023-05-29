QT += core gui widgets opengl

major_version = $$QT_MAJOR_VERSION
# 如果qt版本大于5,需要单独引入一些模块
greaterThan(major_version, 5) {
    message(include openglwidgets)
    QT += openglwidgets
}

CONFIG += c++17
QMAKE_LFLAGS += -flto
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

HEADERS +=

INCLUDEPATH += 3rd_part \
                .

include(gui/gui.pri)
include(function/function.pri)
include(utils/utils.pri)
include(resource/resource.pri)


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../bin

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rd_part/assimp/lib/Windows/ -lassimp \
                                              -L$$PWD/3rd_part/assimp/lib/Windows/zlib/ -lzlibstatic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rd_part/assimp/lib/Windows/ -lassimpd \
#                                                 -L$$PWD/3rd_part/assimp/lib/Windows/zlib/ -lzlibstaticd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rd_part/assimp/lib/Windows/ -lassimp \
                                                 -L$$PWD/3rd_part/assimp/lib/Windows/zlib/ -lzlibstatic


INCLUDEPATH += $$PWD/3rd_part/assimp/include
DEPENDPATH += $$PWD/3rd_part/assimp/include

css.files += $$PWD/assets/css/app.css
css.path  += $$DESTDIR/assets/css/

COPIES += css

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/libassimp.a \
                                                            $$PWD/3rd_part/assimp/lib/Windows/zlib/libzlibstatic.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/libassimpd.a \
#                                                               $$PWD/3rd_part/assimp/lib/Windows/zlib/libzlibstaticd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/libassimp.a \
                                                               $$PWD/3rd_part/assimp/lib/Windows/zlib/libzlibstatic.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/assimp.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/assimpd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rd_part/assimp/lib/Windows/assimp.lib
