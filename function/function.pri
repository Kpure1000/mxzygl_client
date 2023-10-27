HEADERS += \
    $$PWD/assetimporter.h \
    $$PWD/dbtableviewer.h \
    $$PWD/downloader.h \
    $$PWD/indexeditor.h \
    $$PWD/layoutmanager.h \
    $$PWD/metacategory.h \
    $$PWD/modelsearch.h \
    $$PWD/scmanager.h \
    $$PWD/tagsmanager.h \
    $$PWD/typemanager.h \
    $$PWD/usermanager.h \
    $$PWD/versioncontroller.h

include(configer/configer.pri)
include(renderer/renderer.pri)
include(assetloader/assetloader.pri)
include(network/network.pri)

SOURCES += \
    $$PWD/assetimporter.cpp \
    $$PWD/dbtableviewer.cpp \
    $$PWD/downloader.cpp \
    $$PWD/indexeditor.cpp \
    $$PWD/layoutmanager.cpp \
    $$PWD/metacategory.cpp \
    $$PWD/modelsearch.cpp \
    $$PWD/scmanager.cpp \
    $$PWD/tagsmanager.cpp \
    $$PWD/typemanager.cpp \
    $$PWD/usermanager.cpp \
    $$PWD/versioncontroller.cpp
