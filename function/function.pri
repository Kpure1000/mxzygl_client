HEADERS += \
    $$PWD/assetimporter.h \
    $$PWD/metacategory.h \
    $$PWD/modelsearch.h

include(configer/configer.pri)
include(renderer/renderer.pri)
include(assetloader/assetloader.pri)
include(network/network.pri)

SOURCES += \
    $$PWD/assetimporter.cpp \
    $$PWD/metacategory.cpp \
    $$PWD/modelsearch.cpp
