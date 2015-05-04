

#ifndef __Puzzle__PuzzleSprite__
#define __Puzzle__PuzzleSprite__

#include "cocos2d.h"

using namespace cocos2d;

struct PuzzleLocation
{
    int level; //层概念，最外圈用0表示，然后依次向内增大
    int row;   //行
    int column;//列
};

//拼图类
class PuzzleSprite: public CCSprite,public CCTargetedTouchDelegate
{
private:
    int _spriteTag; //表示该精灵的编号，用于计算其上下左右位置
    PuzzleLocation *_PuzzleLcoation; //记录当前拼图块的位置
    CCPoint _endPoint; //表示该精灵需要移动到的位置，即正确的中心位置
    CCPoint _trueAnchorPoint; //正确的锚点
    bool _isMoveToCenter; // 表示是否已经完成移动
    
    bool containsTouchLocation(cocos2d::CCTouch* touch);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
public:
    PuzzleSprite();
    virtual ~PuzzleSprite();
    
    void moveToPoint(CCPoint newPoint);
    
    PuzzleLocation* getPuzzleLoaction();
    void setPuzzleLocation(int level, int row ,int column);
    
    void setMoveToCenter(bool isMoveFinish);
    bool getIsMoveToCenter();
    
    void setEndPoint(CCPoint endPoint);
    CCPoint getEndPoint();
    
    void setTrueAnchorPoint(CCPoint trueAnchorPoint);
    CCPoint getTrueAnchorPoint();
};

#endif /* defined(__Puzzle__PuzzleSprite__) */
