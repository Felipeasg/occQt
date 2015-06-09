/*
*    Copyright (c) 2014 eryar All Rights Reserved.
*
*           File : OccView.h
*         Author : eryar@163.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : Adapte OpenCASCADE view for Qt.
*
*/

#ifndef _OCCVIEW_H_
#define _OCCVIEW_H_

#include <QObject>
#include <QWidget>

#include <OpenGl_GraphicDriver.hxx>
#include <QtOpenGL/QGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>


class QMenu;
class QRubberBand;

//! adapted a QGLWidget for OpenCASCADE viewer.
class OccView : public QGLWidget
{
    Q_OBJECT

public:
    //! mouse actions.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    //! constructor.
    OccView(Handle_AIS_InteractiveContext theContext, QWidget* parent);

signals:
    void selectionChanged(void);

public slots:
    //! operations for the view.
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);

protected:
    // Paint events.
    virtual void paintEvent(QPaintEvent* e);
    virtual void resizeEvent(QResizeEvent* e);

    // Mouse events.
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void wheelEvent(QWheelEvent * e);

    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Popup menu.
    virtual void addItemInPopup(QMenu* theMenu);

protected:
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);

private:

    Handle_V3d_Viewer Viewer(const Standard_ExtString theName,
                                       const Standard_CString theDomain,
                                       const Standard_Real theViewSize,
                                       const V3d_TypeOfOrientation theViewProj,
                                       const Standard_Boolean theComputedMode,
                                       const Standard_Boolean theDefaultComputedMode);
    //! the occ viewer.
    Handle_V3d_View mView;
    Handle_V3d_Viewer myViewer;

    //! the occ context.
    Handle_AIS_InteractiveContext mContext;

    //! save the mouse position.
    Standard_Integer mXmin;
    Standard_Integer mYmin;
    Standard_Integer mXmax;
    Standard_Integer mYmax;

    //! the mouse current mode.
    CurrentAction3d mCurrentMode;

    //! save the degenerate mode state.
    Standard_Boolean mDegenerateModeIsOn;

    //! rubber rectangle for the mouse selection.
    QRubberBand* mRectBand;

};

#endif // _OCCVIEW_H_
