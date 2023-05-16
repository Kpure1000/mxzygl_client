HEADERS += \
    $$PWD/assetimporter.h \
    $$PWD/indexeditor.h \
    $$PWD/layoutmanager.h \
    $$PWD/metacategory.h \
    $$PWD/modelsearch.h \
    $$PWD/modeltagsmanager.h \
    $$PWD/modeltypemanager.h

include(configer/configer.pri)
include(renderer/renderer.pri)
include(assetloader/assetloader.pri)
include(network/network.pri)

SOURCES += \
    $$PWD/assetimporter.cpp \
    $$PWD/indexeditor.cpp \
    $$PWD/layoutmanager.cpp \
    $$PWD/metacategory.cpp \
    $$PWD/modelsearch.cpp \
    $$PWD/modeltagsmanager.cpp \
    $$PWD/modeltypemanager.cpp
