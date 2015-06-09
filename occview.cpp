/*
*    Copyright (c) 2014 eryar All Rights Reserved.
*
*           File : OccView.cpp
*         Author : eryar@163.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : Qt widget for OpenCASCADE viewer.
*/

#include "occview.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>

// occ header files.
#include <V3d_Viewer.hxx>
//#include <WNT_Window.hxx>
#include <Aspect_Handle.hxx>

#include <X11/Xutil.h>
#include <Graphic3d_GraphicDriver.hxx>
#include <Xw_Window.hxx>

// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftModifier

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlModifier


OccView::OccView( Handle_AIS_InteractiveContext theContext, QWidget* parent )
    : QGLWidget(parent),
    mContext(theContext),
    mXmin(0),
    mXmax(0),
    mYmin(0),
    mYmax(0),
    mDegenerateModeIsOn(Standard_True),
    mCurrentMode(CurAction3d_DynamicRotation),
    mRectBand(NULL)
{
    // Create the view.
    //mView = theContext->CurrentViewer()->CreateView();
    TCollection_ExtendedString a3DName ("Visu3D");

    myViewer = Viewer (a3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos, Standard_True, Standard_True);

    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myContext = new AIS_InteractiveContext(myViewer);

    if ( myView.IsNull() )
      mView = myContext->CurrentViewer()->CreateView();


#if defined(_WIN32) || defined(__WIN32__)
  Aspect_Handle aWindowHandle = (Aspect_Handle )winId();
  Handle(WNT_Window) hWnd = new WNT_Window (aWindowHandle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
  NSView* aViewHandle = (NSView* )winId();
  Handle(Cocoa_Window) hWnd = new Cocoa_Window (aViewHandle);
#else
  Window aWindowHandle = (Window )winId(); //Include X11/Xutil.h
  Handle(Aspect_DisplayConnection) aDispConnection = myContext->CurrentViewer()->Driver()->GetDisplayConnection(); //Include Graphic3d_GraphicDriver.hxx
  Handle(Xw_Window) hWnd = new Xw_Window (aDispConnection, aWindowHandle); //Include Xw_Window.hxx
#endif // WNT

    // Attaching the window to the view.
    // Portable in principle, but if you use it you are probably about to do something
    // non-portable. Be careful. Gets the window system identifier of the widget by winId().
  #if defined(_WIN32) || defined(__WIN32__)
    HWND winID = (HWND) (winId());
    Handle_WNT_Window hWnd = new WNT_Window(winID);
#endif

    mView->SetWindow(hWnd);
    if (!hWnd->IsMapped())
    {
        hWnd->Map();
    }

    mView->SetBackgroundColor(Quantity_NOC_BLACK);
    mView->MustBeResized();
    mView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );
}

void OccView::paintEvent( QPaintEvent* e )
{
    // eliminate the warning C4100: 'e' : unreferenced formal parameter
    UNREFERENCED_PARAMETER(e);

    mView->Redraw();
}

void OccView::resizeEvent( QResizeEvent* e )
{
    UNREFERENCED_PARAMETER(e);

    if( !mView.IsNull() )
    {
        mView->MustBeResized();
    }
}

void OccView::fitAll( void )
{
    mView->FitAll();
    mView->ZFitAll();
    mView->Redraw();
}

void OccView::reset( void )
{
    mView->Reset();
}

void OccView::pan( void )
{
    mCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::zoom( void )
{
    mCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate( void )
{
    mCurrentMode = CurAction3d_DynamicRotation;
}


void OccView::mousePressEvent( QMouseEvent* e )
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
}

void OccView::mouseReleaseEvent( QMouseEvent* e )
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
}

void OccView::mouseMoveEvent( QMouseEvent * e )
{
    onMouseMove(e->buttons(), e->pos());
}

void OccView::wheelEvent( QWheelEvent * e )
{
    onMouseWheel(e->buttons(), e->delta(), e->pos());
}

void OccView::onLButtonDown( const int theFlags, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);

    // Save the current mouse coordinate in min.
    mXmin = thePoint.x();
    mYmin = thePoint.y();
    mXmax = thePoint.x();
    mYmax = thePoint.y();

}

void OccView::onMButtonDown( const int theFlags, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);

    // Save the current mouse coordinate in min.
    mXmin = thePoint.x();
    mYmin = thePoint.y();
    mXmax = thePoint.x();
    mYmax = thePoint.y();

    if (mCurrentMode == CurAction3d_DynamicRotation)
    {
        mView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown( const int theFlags, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);
    UNREFERENCED_PARAMETER(thePoint);
}

void OccView::onMouseWheel( const int theFlags, const int theDelta, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);

    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    mView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup( QMenu* theMenu )
{
    UNREFERENCED_PARAMETER(theMenu);
}

void OccView::popup( const int x, const int y )
{
    UNREFERENCED_PARAMETER(x);
    UNREFERENCED_PARAMETER(y);
}

void OccView::onLButtonUp( const int theFlags, const QPoint thePoint )
{
    // Hide the QRubberBand
    if (mRectBand)
    {
        mRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == mXmin && thePoint.y() == mYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void OccView::onMButtonUp( const int theFlags, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);

    if (thePoint.x() == mXmin && thePoint.y() == mYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void OccView::onRButtonUp( const int theFlags, const QPoint thePoint )
{
    UNREFERENCED_PARAMETER(theFlags);

    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove( const int theFlags, const QPoint thePoint )
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(mXmin, mYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MidButton)
    {
        switch (mCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            mView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            mView->Zoom(mXmin, mYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            mView->Pan(thePoint.x() - mXmax, mYmax - thePoint.y());
            mXmax = thePoint.x();
            mYmax = thePoint.y();
            break;
        }
    }

}

void OccView::dragEvent( const int x, const int y )
{
    mContext->Select( mXmin, mYmin, x, y, mView );

    emit selectionChanged();
}

void OccView::multiDragEvent( const int x, const int y )
{
    mContext->ShiftSelect( mXmin, mYmin, x, y, mView );

    emit selectionChanged();

}

void OccView::inputEvent( const int x, const int y )
{
    UNREFERENCED_PARAMETER(x);
    UNREFERENCED_PARAMETER(y);

    mContext->Select();

    emit selectionChanged();
}

void OccView::multiInputEvent( const int x, const int y )
{
    UNREFERENCED_PARAMETER(x);
    UNREFERENCED_PARAMETER(y);

    mContext->ShiftSelect();

    emit selectionChanged();
}

void OccView::moveEvent( const int x, const int y )
{
    mContext->MoveTo(x, y, mView);
}

void OccView::multiMoveEvent( const int x, const int y )
{
    mContext->MoveTo(x, y, mView);
}

void OccView::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!mRectBand)
    {
        mRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        mRectBand->setStyle(QStyleFactory::create("windows"));
    }

    mRectBand->setGeometry(aRect);
    mRectBand->show();
}

void OccView::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    mView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

Handle_V3d_Viewer OccView::Viewer(const Standard_ExtString theName, const Standard_CString theDomain, const Standard_Real theViewSize, const V3d_TypeOfOrientation theViewProj, const Standard_Boolean theComputedMode, const Standard_Boolean theDefaultComputedMode)
{
    static Handle(OpenGl_GraphicDriver) aGraphicDriver;

    if (aGraphicDriver.IsNull())
    {
      Handle(Aspect_DisplayConnection) aDisplayConnection;
  #if !defined(_WIN32) && !defined(__WIN32__) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX))
      aDisplayConnection = new Aspect_DisplayConnection (qgetenv ("DISPLAY").constData());
  #endif
      aGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);
    }

    return new V3d_Viewer (aGraphicDriver,
                           theName,
                           theDomain,
                           theViewSize,
                           theViewProj,
                           Quantity_NOC_GRAY30,
                           V3d_ZBUFFER,
                           V3d_GOURAUD,
                           V3d_WAIT,
                           theComputedMode,
                           theDefaultComputedMode,
                           V3d_TEX_NONE);
}
