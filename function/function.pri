HEADERS += \
    $$PWD/assetimporter.h \
    $$PWD/modelsearch.h

include(configer/configer.pri)
include(renderer/renderer.pri)
include(assetloader/assetloader.pri)
include(network/network.pri)

SOURCES += \
    $$PWD/assetimporter.cpp \
    $$PWD/modelsearch.cpp
