

#ifndef __Puzzle__PuzzleScene__
#define __Puzzle__PuzzleScene__

#include "cocos2d.h"
using namespace cocos2d;

class PuzzleScene : public cocos2d::CCLayer
{
public:
    PuzzleScene();
    ~PuzzleScene();
    
    virtual bool init();
    static cocos2d::CCScene* scene(const char* picImageName,int fragsNum);
    
    CREATE_FUNC(PuzzleScene);
    
private:
    CCArray* _fragsArray;  //保存切图的数组
    CCSprite* _pic;        //切图的原图
    CCLayerColor* _showFragsLayer; // 展示图片的区域块
    
    CCSize _winSize;
    int _fragsNum;
    
    const char* _picImageName; //原图的地址
    void setImageName(const char* picImageName);
    void setFragsNum(int fragsNum);
    
    virtual void onEnter();
    
    void prepareFrags();
    void displayPic();
    void displayFrags();
    
};


#endif /* defined(__Puzzle__PuzzleScene__) */
