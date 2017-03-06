#ifndef OBJECTSELECTOR_H
#define OBJECTSELECTOR_H

#include <QWidget>
#include <cl2/ui/imageview.h>
#include "fsm.h"
#include <QGraphicsScene>
#include <QPoint>
#include <QPolygon>



class ObjectSelector : public ObjectSelectorInterface
{
  Q_OBJECT
  QGraphicsScene * _scene;
  FSM  _markFsm;
  QColor _selectionColor;
  QPoint _offset;
  bool _active;

public:
  ObjectSelector( QGraphicsScene * scene );
  virtual ~ObjectSelector( );
  void reset();
  void accept();
  void mouseMoved( QPoint );
  void mousePressed( QPoint );
  void mouseReleased( QPoint );
  bool isSelecting() const;
  void setSelectionColor( QColor c );
  QColor selectionColor(  )const;
  void setImageOffset( QPoint o ){ _offset = o; }
  void setActive( bool a ){ _active = a; }
  bool isActive(  )const{ return _active; }

private:
  void updatePolygon();
signals:
  void canceled();
  void accepted(QPolygon);
};

#endif // OBJECTSELECTOR_H
