

#ifndef __puzzle__PuzzleSpriteFactory__
#define __puzzle__PuzzleSpriteFactory__

#include "PuzzleSprite.h"

//拼图类
class PuzzleSpriteFactory: public CCObject
{
private:
    CCSprite* _sourceSprite; //背景图片
    int _cutNum;  //具体切片一条边，总数字为其平方
    float _cutWidth; //具体单片的宽度
    
    float _scale; //需要缩放的大小
    
    char* getMaskSpriteName(int row, int column);
    
    //maskName 用来表示模板的名字，同时代表了模板的样式 X为直线，0未内凹，1为外凸
    CCPoint calculatePoint(const char* maskName);
    
    CCSprite* createMaskedSprite(const char * maskName);
    PuzzleSprite* createPuzzleSprite(CCSprite* mask, const char * maskName, int row ,int column);
    
public:
    PuzzleSpriteFactory(const char* sourceName, float cutNum);

    virtual ~PuzzleSpriteFactory();
    
    CCArray* createPuzzle();
};


#endif /* defined(__puzzle__PuzzleSpriteFactory__) */
