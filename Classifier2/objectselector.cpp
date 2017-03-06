#include "objectselector.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QList>
#include <QPoint>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QPolygon>
#include "fsm.h"



State Idle("Idle"),
      FirstPoint("FirstPoint"),
      SecondPoint("SecondPoint"),
      AreaMarking("AreaMarking"),
      AreaAccept("AreaAccept"), 
      ANY("ANY", true);

enum {MouseDn, MouseUp, MouseMove, Accept, Reset};


QList<QPoint> _path;
QGraphicsLineItem *_rubberLine = 0;
QGraphicsPolygonItem *_currentPolygon = 0;
bool marking = false;
bool accepted = false;


void resetPolygon()
{
	
  if ( _currentPolygon )
    _currentPolygon->scene()->removeItem( _currentPolygon );
  _currentPolygon = 0;

  if ( _rubberLine )
    _rubberLine->scene()->removeItem( _rubberLine );
  _rubberLine = 0;

  _path.clear();
  marking = false;
  
}

void ObjectSelector::updatePolygon()
{
  
  if ( _rubberLine )
  {
    _rubberLine->scene()->removeItem( _rubberLine );
    _rubberLine = 0;
  }

  if ( _currentPolygon )
  {
    QGraphicsScene * _scene = _currentPolygon->scene();
    _currentPolygon->scene()->removeItem( _currentPolygon );
    _currentPolygon = _scene->addPolygon(QPolygon( _path.toVector() ), QPen("blue"),QBrush(_selectionColor));
    _currentPolygon->setOpacity(.4);
  }

}

bool ObjectSelector::isSelecting() const
{
  return marking;
}

ObjectSelector * self=0;
void idleEnter()
{
	_path.clear(); marking = false;
}
void AreaMarkingEnter()
{ 
	marking = true; 
}

void AreaMarkingExit()
{ 
	marking = false; 
}

void ZeroPolygon(){ _currentPolygon = 0; }

ObjectSelector::ObjectSelector( QGraphicsScene *scene  ) :_scene(scene)
{
  self = this;
  Idle.onEnter = &idleEnter;//ACTION{ _path.clear(); marking = false; };
  AreaMarking.onEnter = &AreaMarkingEnter;//ACTION{marking = true; };
  AreaMarking.onExit =  AreaMarkingExit;//ACTION{marking = false; };
  AreaAccept.onEnter =  0;//ACTION{  };


  
  _markFsm(ANY,         "  -->  ",  Idle,        "ON:", Reset, &resetPolygon );
  _markFsm(Idle,        "  -->  ",  FirstPoint,  "ON:", MouseDn);
  _markFsm(FirstPoint,  "  -->  ",  SecondPoint, "ON:", MouseUp);
  _markFsm(SecondPoint, "  -->  ",  AreaMarking, "ON:", MouseMove);
  _markFsm(AreaMarking, "  -->  ",  AreaMarking, "ON:", MouseMove);
  _markFsm(AreaMarking, "  -->  ",  AreaAccept,  "ON:", MouseUp);
  _markFsm(SecondPoint, "  -->  ",  Idle,        "ON:", MouseUp);
  _markFsm(AreaAccept,  "  -->  ",  Idle,        "ON:", Accept, &ZeroPolygon );
  _markFsm.setStartState(Idle);
  _markFsm.start();
  
}

ObjectSelector::~ObjectSelector( )
{

}

void ObjectSelector::setSelectionColor( QColor c )
{
  _selectionColor = c;
}

QColor ObjectSelector::selectionColor(  )const
{
  return _selectionColor;
}

void ObjectSelector::accept( )
{
	if (!_active) return;

	if (_path.size() > 2)
	{
	  _path.removeFirst(); _path << _path.at(0); 
		updatePolygon();

	  if ( !isSelecting() && _currentPolygon )
	  {
		  emit accepted(  _currentPolygon->polygon().toPolygon() );
	  }
	  _markFsm.processEvent( Accept );
	}
}

void ObjectSelector::reset( )
{
	if (!_active) return;

   _markFsm.processEvent( Reset );
   if ( !isSelecting() )
   {
     emit canceled( );
   }
}

void ObjectSelector::mouseMoved(QPoint pos)
{
	if (!_active) return;

  _markFsm.processEvent( MouseMove );
  if ( _path.size() == 1 )
  {
    if ( _rubberLine )
    {
      _scene->removeItem(_rubberLine);
    }
	_rubberLine = _scene->addLine( _path.at(0).x(),_path.at(0).y(),  pos.x(),  pos.y() );
  }
  else if ( _path.size() >= 2 && marking )
  {
    QPolygon old( _path.toVector() );
    QPolygon added;added <<_path.at(0) << _path.last() <<  pos;

    if ( old.intersected( added ).isEmpty() )
    {
      _path << pos;
      if ( _currentPolygon )
      {
        _scene->removeItem( _currentPolygon );
      }
      _currentPolygon = _scene->addPolygon(QPolygon( _path.toVector() ), QPen("blue"),QBrush(_selectionColor));
      _currentPolygon->setOpacity(.4);
    }
  }
  
}

void ObjectSelector::mousePressed( QPoint pos )
{
	if (!_active) return;

   _path << pos;
   _markFsm.processEvent( MouseDn );
}

void ObjectSelector::mouseReleased( QPoint pos )
{
	if (!_active) return;

   _markFsm.processEvent( MouseUp );
}
