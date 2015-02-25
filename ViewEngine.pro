TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
INCLUDEPATH += "C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2-2.0.3//i686-w64-mingw32//include" \
"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//freeglut//include"

LIBS += -L"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//freeglut//lib" \
-L"C://!!!!!!!!!!!!!!!!!!!!!!!!!!!//PROGRAMMING_BOX//ProgrammingLibs//SDL2-2.0.3//i686-w64-mingw32//lib" \
-lopengl32 \
-lglew32 \
-lmingw32 \
-lSDL2main \
-lSDL2

HEADERS += \
    linalg/mat.h \
    linalg/vec.h \
    view/RViewModels.h \
    view/RViewModelsGL.h \
    base/RBase.h \
    base/RModels.h \
    view/RVertex.h \
    base/REventer.h \
    base/RFileloader.h \
    base/RFileParser.hpp \
    view/RPassDesc.h \
    view/RAnim.h \
    view/RDrawable.h \
    view/RLight.h \
    view/RTextures.h \
    view/RAnimGL.h \
    view/GL.h \
    view/RTexturesGL.h \
    view/RLightGL.h \
    view/RPassGL.h \
    view/RDrawableGL.h \
    view/RSceneDrawler.h

