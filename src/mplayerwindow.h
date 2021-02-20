/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>
    Copyright (C) 2010-2021 Rupesh Sreeraman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef MPLAYERWINDOW_H
#define MPLAYERWINDOW_H

#include <QWidget>
#include <QSize>
#include <QPoint>

#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <rphlabelex.h>


class rphLabelEx;
class QWidget;
class QLabel;
class QKeyEvent;
class QTimer;

#define ZOOM_STEP 0.05
#define ZOOM_MIN 0.5

#define DELAYED_RESIZE 0
#define NEW_MOUSE_CHECK_POS 1

//! Screen is a widget that hides the mouse cursor after some seconds if not moved.

class Screen : public rphLabelEx
{
    Q_OBJECT

public:
    Screen(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~Screen();
    bool showtext;
    void  setShowText(bool en){showtext=en;}
protected:

#if !NEW_MOUSE_CHECK_POS
    virtual void mouseMoveEvent( QMouseEvent * e );
#endif
    virtual void paintEvent ( QPaintEvent * e );

protected slots:
    virtual void checkMousePos();

private:
#if NEW_MOUSE_CHECK_POS
    QPoint mouse_last_position;
#else
    //QPoint cursor_pos, last_cursor_pos;
#endif
};

//! MplayerLayer can be instructed to not delete the background.

class MplayerLayer : public Screen
{
    Q_OBJECT 
public:
    MplayerLayer(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~MplayerLayer();

public slots:
    //! If b is true, the background of the widget will be repainted as usual.
    /*! Otherwise the background will not repainted when a video is playing. */
    void setRepaintBackground(bool b);

    //! Return true if repainting the background is allowed.
        bool repaintBackground() { return repaint_background; }


    //! Should be called when a file has started.
    /*! It's needed to know if the background has to be cleared or not. */
    void playingStarted();
    //! Should be called when a file has stopped.
    void playingStopped();
private:
    bool playing;
     bool repaint_background;
protected:

    virtual void paintEvent ( QPaintEvent * e );
};


class MplayerWindow : public Screen
{
    Q_OBJECT

public:
    MplayerWindow( QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~MplayerWindow();
    MplayerLayer * mplayerlayer;
    void showLogo( bool b);

    MplayerLayer * videoLayer() { return mplayerlayer; };

    void setResolution( int w, int h);
    void setAspect( double asp);
    void setMonitorAspect(double asp);
    void updateVideoWindow();
    void setColorKey(QColor c);
    void setOffsetX( int );
    int offsetX();
    void setOffsetY( int );
    int offsetY();
    void setZoom( double );
    double zoom();
    void allowVideoMovement(bool b) { allow_video_movement = b; };
    bool isVideoMovementAllowed() { return allow_video_movement; };
    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint() const;
    virtual bool eventFilter( QObject * watched, QEvent * event );
    void setShowVideo(bool en){showvideo=en;update();}
    long int getWinid(){return (long int)this->winId();}

public slots:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void incZoom();
    void decZoom();

#if DELAYED_RESIZE
protected slots:
    void resizeLater();
#endif

protected:
    virtual void retranslateStrings();
    virtual void changeEvent ( QEvent * event ) ;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void resizeEvent( QResizeEvent * e);
    virtual void mouseReleaseEvent( QMouseEvent * e);
    virtual void mouseDoubleClickEvent( QMouseEvent * e );
    virtual void wheelEvent( QWheelEvent * e );
    void moveLayer( int offset_x, int offset_y );
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent *);
signals:
    //void rightButtonReleased( QPoint p );
    void doubleClicked();
    void leftClicked();
    void rightClicked();
    void middleClicked();
    void xbutton1Clicked(); // first X button
    void xbutton2Clicked(); // second X button
    void keyPressed(QKeyEvent * e);
    void wheelUp();
    void wheelDown();
    void mouseMoved(QPoint);
    void mouseMoveEventsg(QMouseEvent *e);
    void mousewheelsg(QWheelEvent * event);
    void resizeVideo(int wid, int hei);

protected:
    int video_width, video_height;
    double aspect;
    double monitoraspect;
    QLabel * logo;
    QMovie * movie;
    // Zoom and moving
    int offset_x, offset_y;
    double zoom_factor;

    // Original pos and dimensions of the mplayerlayer
    // before zooming or moving
    int orig_x, orig_y;
    int orig_width, orig_height;

    bool allow_video_movement;
    bool showvideo;
    bool bshowLogo;

#if DELAYED_RESIZE
    QTimer * resize_timer;
#endif
};


#endif

