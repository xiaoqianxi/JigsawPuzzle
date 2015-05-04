

#include "PuzzleSpriteFactory.h"
#include "PuzzleSprite.h"

//这些边都是用左边取的，当为右边时，需要求反
char maskLeft[10][10] ={
    {'X','0','0','1','0','0','0','0','0','0'},
    {'X','1','1','1','1','1','0','0','0','0'},
    {'X','1','0','0','0','0','0','1','1','1'},
    {'X','0','1','0','0','1','0','0','0','1'},
    {'X','0','0','1','1','1','1','0','1','0'},
    {'X','1','1','0','1','0','0','1','1','1'},
    {'X','1','1','1','0','1','0','1','1','1'},
    {'X','0','0','1','1','1','1','1','1','0'},
    {'X','0','1','1','1','1','0','1','1','0'},
    {'X','0','1','0','0','1','1','0','0','1'}};

//这些边都是用上边取的，当为下边时，需要求反
char maskUp[10][10]={
    {'X','X','X','X','X','X','X','X','X','X'},
    {'0','1','0','1','0','0','1','1','1','0'},
    {'0','0','0','0','0','0','1','1','0','0'},
    {'0','1','0','1','0','0','1','0','0','0'},
    {'0','1','0','1','0','0','0','1','1','1'},
    {'0','1','0','1','0','1','1','1','0','1'},
    {'1','0','1','1','0','0','0','0','1','1'},
    {'1','1','0','0','1','1','1','0','1','0'},
    {'0','0','1','0','1','1','0','0','0','1'},
    {'1','0','0','0','0','1','1','1','0','0'}
};

PuzzleSpriteFactory::PuzzleSpriteFactory(const char* sourceName, float cutNum)
{
    _sourceSprite = CCSprite::create(sourceName);
    assert(_sourceSprite);
    _sourceSprite->retain();
    
    _cutNum = cutNum;
    
    //取到要达到的长度和需要切分的长度
    _cutWidth = _sourceSprite->getContentSize().width/cutNum;
    float pngWidth = 192;
    
    _scale = _cutWidth/pngWidth;
}

PuzzleSpriteFactory::~PuzzleSpriteFactory()
{
    _sourceSprite->release();
    _sourceSprite = NULL;
}

CCArray* PuzzleSpriteFactory::createPuzzle()
{
    if (_sourceSprite!=NULL) {
        CCArray* puzzleArray = CCArray::create();
        for (int row = 0; row<_cutNum; row++) {
            for (int column = 0; column<_cutNum; column++) {

                //生成图片,设置切图参数
                int levelTag = (row*column)%(_cutNum-1);
                char* maskName = this->getMaskSpriteName(row, column);
                
                CCSprite* maskSprite = this->createMaskedSprite(maskName);
                PuzzleSprite *puzzleSprite = this->createPuzzleSprite(maskSprite,maskName,row,column);
                
                puzzleSprite->setPuzzleLocation(levelTag,row,column);
                puzzleArray->addObject(puzzleSprite);
            }
        }
        return puzzleArray;
    }
    return NULL;
}

//将两张背景图片压在一起，一张阴影，一张白色底图
CCSprite* PuzzleSpriteFactory::createMaskedSprite(const char * maskName)
{
    //得到mask的具体文件名字, M为白色底图，B为阴影
    char maskBName[40] = {0};
    char maskMName[40] = {0};
    std::sprintf(maskMName,"maskImage/M%s.png",maskName);
    std::sprintf(maskBName,"maskImage/B%s.png",maskName);
    
    CCSprite* maskB = CCSprite::create(maskBName);
    CCSprite* maskM = CCSprite::create(maskMName);
    assert(maskB);
    assert(maskM);
    
    //计算mask的位置
    CCSize maskContent = maskM->getContentSize();
    
    CCRenderTexture * rt = CCRenderTexture::create(maskContent.width, maskContent.height, kTexture2DPixelFormat_RGBA8888);
    
    maskM->setAnchorPoint(ccp(0.5, 0.5));
    maskB->setAnchorPoint(ccp(0.5, 0.5));
    maskM->setPosition(ccp(maskContent.width / 2, maskContent.height / 2));
    maskB->setPosition(ccp(maskContent.width / 2, maskContent.height / 2));
    
    ccBlendFunc blendFunc = { GL_SRC_ALPHA, GL_DST_ALPHA };
    maskM->setBlendFunc(blendFunc);
    
    ccBlendFunc blendFunc2 = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
    maskB->setBlendFunc(blendFunc2);
    
    
    rt->begin();
    maskM->visit();
    maskB->visit();
    rt->end();
    
    
	CCSprite *retval = new CCSprite();
	if (retval && retval->initWithTexture(rt->getSprite()->getTexture()))
	{
		retval->setFlipY(true);
		retval->autorelease();
        
		return retval;
	}
	CC_SAFE_DELETE(retval);
	return NULL;
}

//通过模板创建拼图块, 这里拼图的结果，是生成了模板乘以_scale大小的底图
PuzzleSprite* PuzzleSpriteFactory::createPuzzleSprite(CCSprite* mask, const char * maskName, int row ,int column)
{
    assert(mask);
    assert(_sourceSprite);
    
    //计算mask的位置
    CCSize srcContent = _sourceSprite->getContentSize();
    CCSize maskContent = CCSizeMake(mask->getContentSize().width*_scale, mask->getContentSize().height*_scale);
    
    CCRenderTexture * rt = CCRenderTexture::create(maskContent.width, maskContent.height, kTexture2DPixelFormat_RGBA8888);
    
    float fragWidth = srcContent.width / _cutNum;
    float fragHeight = srcContent.height / _cutNum;
    
    mask->setScale(_scale);
    
    mask->setAnchorPoint(ccp(0.5,0.5));
    CCPoint fixPoint = ccp(calculatePoint(maskName).x * _scale, calculatePoint(maskName).y * _scale);
    CCPoint endPoint = ccp(column * fragWidth + fixPoint.x+maskContent.width/2,srcContent.height - row* fragHeight - fixPoint.y-maskContent.height/2);
    
    mask->setPosition(ccp(maskContent.width/2, maskContent.height/2));
    
    ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO };
    mask->setBlendFunc(blendFunc2);

    float moveX = srcContent.width/2-endPoint.x;
    float moveY = srcContent.height/2-endPoint.y;
    
    _sourceSprite->setAnchorPoint(ccp(0.5, 0.5));
    _sourceSprite->setPosition(ccp(maskContent.width/2+moveX, maskContent.height / 2+moveY));
    
    
    ccBlendFunc blendFunc = { GL_DST_ALPHA, GL_ZERO };
    _sourceSprite->setBlendFunc(blendFunc);
    
    rt->begin();
    mask->visit();
    _sourceSprite->visit();
    rt->end();
    
    
	PuzzleSprite *retval = new PuzzleSprite();
	if (retval && retval->initWithTexture(rt->getSprite()->getTexture()))
	{
		retval->setFlipY(true);
		retval->autorelease();
        
        //endPoint的是以ccp(0.5,0.5)为锚点的
        retval->setTrueAnchorPoint(ccp(0.5, 0.5));
        retval->setEndPoint(endPoint);
		return retval;
	}
	CC_SAFE_DELETE(retval);
	return NULL;
}

CCPoint PuzzleSpriteFactory::calculatePoint(const char* maskName)
{
	float x = 0;
	float y = 0;
    
	if(maskName[0] == '1')
		x = -48;
	if(maskName[1] == '1')
		y = -48;
	if(maskName[0] == '0')
		x = -8;
	if(maskName[1] == '0')
		y = -8;
    
	return ccp(x, y);
}

char* PuzzleSpriteFactory::getMaskSpriteName(int row, int column)
{
    if (row<10 && column<10) {
        char left = maskLeft[row][column];
        switch (left) {
            case '0':
                left = '1';
                break;
            case '1':
                left = '0';
            default:
                left = left;
                break;
        }
        char right = column<_cutNum-1 && column<9 ? maskLeft[row][column+1]:'X';
        
        char up = maskUp[row][column];
        switch (up) {
            case '0':
                up = '1';
                break;
            case '1':
                up = '0';
            default:
                up = up;
                break;
        }
        char down = row<_cutNum-1 && row<9 ? maskUp[row+1][column]:'X';
        
        char* maskName = new char;
        *maskName = left;
        *(maskName+1)=up;
        *(maskName+2)=right;
        *(maskName+3)=down;
        *(maskName+4)='\0';
        return maskName;
    }
    return NULL;
}


