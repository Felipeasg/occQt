#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T19:09:34
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = occQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    occview.cpp

HEADERS  += mainwindow.h \
    occview.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/oce

win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKernel.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKMath.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKBRep.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKTopAlgo.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKPrim.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKBO.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKOffset.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKService.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKV3d.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKOpenGl.lib
win32:  LIBS += D:\OpenCASCADE6.8.0\opencascade-6.8.0\win32\vc12\libd\TKFillet.lib

unix: INCLUDEPATH += /usr/include/oce

unix: LIBS += -L/usr/lib64/oce -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset \
        -lfreeimage -lX11
