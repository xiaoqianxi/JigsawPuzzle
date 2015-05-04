

#include "PuzzleSprite.h"

#define PUZZLEGAP 20

PuzzleSprite:: PuzzleSprite(){
    _spriteTag = -1;
    _endPoint = CCPointZero;
    _trueAnchorPoint = CCPointMake(0, 1);
    _isMoveToCenter = false;
    
    _PuzzleLcoation = new PuzzleLocation();
}

PuzzleSprite:: ~PuzzleSprite(){
    delete _PuzzleLcoation;
    CCLog("delete PuzzleSprite %d",_spriteTag);
}


void PuzzleSprite::moveToPoint(CCPoint newPoint)
{
    this->runAction(CCMoveTo::create(0.1, newPoint));
}


PuzzleLocation* PuzzleSprite::getPuzzleLoaction()
{
    return _PuzzleLcoation;
}

void PuzzleSprite::setPuzzleLocation(int level, int row ,int column)
{
    _PuzzleLcoation->level = level;
    _PuzzleLcoation->row = row;
    _PuzzleLcoation->column = column;
}

void PuzzleSprite::setMoveToCenter(bool isMoveFinish)
{
    _isMoveToCenter = isMoveFinish;
}

bool PuzzleSprite::getIsMoveToCenter()
{
    return _isMoveToCenter;
}

void PuzzleSprite::setEndPoint(CCPoint endPoint)
{
    _endPoint = endPoint;
}

CCPoint PuzzleSprite::getEndPoint()
{
    return _endPoint;
}

void PuzzleSprite::setTrueAnchorPoint(CCPoint trueAnchorPoint)
{
    _trueAnchorPoint = trueAnchorPoint;
}

CCPoint PuzzleSprite::getTrueAnchorPoint()
{
    return _trueAnchorPoint;
}


bool PuzzleSprite::containsTouchLocation(cocos2d::CCTouch* touch)
{
    
    CCSize s = this->getContentSize();
    CCRect rect =  CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
    
    return rect.containsPoint(this->convertTouchToNodeSpaceAR(touch));
}

void PuzzleSprite::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void PuzzleSprite::onExit()
{
    CCSprite::onExit();
}


bool PuzzleSprite::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if ( !containsTouchLocation(touch)){
        return false;
    }
    return true;
}

void PuzzleSprite::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
    if (this->getParent()!=NULL) {
        CCPoint touchPoint = this->getParent()->convertTouchToNodeSpace(touch);
        setPosition( ccp(touchPoint.x, touchPoint.y) );
    }
}

void PuzzleSprite::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (abs(this->getPosition().x-_endPoint.x) <=PUZZLEGAP && abs(this->getPosition().y-_endPoint.y)<=PUZZLEGAP) {
        this->moveToPoint(_endPoint);
    }
}

void PuzzleSprite::touchDelegateRetain()
{
    this->retain();
}

void PuzzleSprite::touchDelegateRelease()
{
    this->release();
}



