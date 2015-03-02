QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
#TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += VIEWLIBRARY_EXPORT
SOURCES += main.cpp
INCLUDEPATH += "C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2-2.0.3//i686-w64-mingw32//include" \
"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//freeglut//include" \
"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2//image"

LIBS += -L"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//freeglut//lib" \
-L"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2-2.0.3//i686-w64-mingw32//lib" \
-L"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2//image//lib" \
-lopengl32 \
-lglew32 \
-lmingw32 \
-lSDL2main \
-lSDL2 \
-lSDL2_image
HEADERS += \
    base/Parallel.h \
    base/RBase.h \
    base/REventer.h \
    base/RFileloader.h \
    base/RFileParser.hpp \
    base/RModels.h \
    linalg/mat.h \
    linalg/vec.h \
    view/3dgl/RAnim.h \
    view/3dgl/RAnimGL.h \
    view/3dgl/RDrawable.h \
    view/3dgl/RDrawableGL.h \
    view/3dgl/RLight.h \
    view/3dgl/RLightGL.h \
    view/3dgl/RPassDesc.h \
    view/3dgl/RPassGL.h \
    view/3dgl/RTextures.h \
    view/3dgl/RTexturesGL.h \
    view/3dgl/RVertex.h \
    view/3dgl/RViewModels.h \
    view/3dgl/RViewModelsGL.h \
    view/gui/gl/GUIRendererGL.h \
    view/GL.h \
    view/ViewEngineGL.hpp \
    view/ViewInterface.h \
    view/WindowGL.hpp \
    view/3dgl/RCamera.h \
    view/export/RCamera.h \
    view/export/RViewModels.h \
    view/export/ViewInterface.h \
    view/3dgl/ShaderSpecs.h

