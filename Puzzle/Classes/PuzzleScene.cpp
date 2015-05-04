

#include "PuzzleScene.h"
#include "PuzzleSpriteFactory.h"
#include "VisibleRect.h"
#include "PuzzleSprite.h"

USING_NS_CC;

#define maxFragsNum 10

PuzzleScene::PuzzleScene()
{
    _fragsArray = NULL;
    _pic = NULL;
    _picImageName = NULL;
    _fragsNum = 2;
}

PuzzleScene::~PuzzleScene()
{
    if (_fragsArray!=NULL) {
        _fragsArray->release();
        _fragsArray = NULL;
    }
    if (_pic!=NULL) {
        _pic->release();
        _pic = NULL;
    }
    if (_showFragsLayer!=NULL) {
        _showFragsLayer->release();
        _showFragsLayer = NULL;
    }
    delete _picImageName;
    _picImageName = NULL;
}

CCScene* PuzzleScene::scene(const char* picImageName, int fragsNum)
{
    CCScene *scene = CCScene::create();
    
    PuzzleScene *layer = PuzzleScene::create();
    layer->setImageName(picImageName);
    layer->setFragsNum(fragsNum);
    scene->addChild(layer);
    return scene;
}

void PuzzleScene::setImageName(const char* picImageName)
{
    _winSize = CCDirector::sharedDirector()->getWinSize();
    
    _picImageName = picImageName;
    
    _pic = CCSprite::create(_picImageName);
    _pic->setPosition(VisibleRect::center());
    _pic->retain();
    this->addChild(_pic, 1);
    
    CCSize imageSize = _pic->getContentSize();

    _showFragsLayer = CCLayerColor::create(ccc4(255, 255, 255, 0), imageSize.width, imageSize.height);
    _showFragsLayer->setContentSize(imageSize);
    _showFragsLayer->setPosition((_winSize.width-imageSize.width)/2, (_winSize.height-imageSize.height)/2);
    _showFragsLayer->retain();
    this->addChild(_showFragsLayer,0);
}

void PuzzleScene::setFragsNum(int fragsNum)
{
    _fragsNum = MAX(2, fragsNum);
    _fragsNum = MIN(_fragsNum,maxFragsNum);
}

bool PuzzleScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSprite* pBG =  CCSprite::create("bk@2x.png");
    pBG->setPosition(VisibleRect::center());
    this->addChild( pBG );
    
    return true;
}

void PuzzleScene::onEnter()
{
	CCLayer::onEnter();
	setTouchEnabled(true);
	PuzzleScene::prepareFrags();
	
	CCActionInstant *displayPicCallback = CCCallFunc::create(this, callfunc_selector(PuzzleScene::displayPic));
	CCActionInstant *displayFragsCallback = CCCallFunc::create(this, callfunc_selector(PuzzleScene::displayFrags));
    
	this->runAction(CCSequence::create(CCDelayTime::create(1), displayPicCallback, 0));
	this->runAction(CCSequence::create(CCDelayTime::create(2), displayFragsCallback, 0));
}


void PuzzleScene::prepareFrags()
{
    PuzzleSpriteFactory * factory = new PuzzleSpriteFactory(_picImageName,_fragsNum);
    _fragsArray =  factory->createPuzzle();
    _fragsArray->retain();
    
    for (int i = 0; i< _fragsArray->count(); i++) {
        PuzzleSprite* maskSprite = (PuzzleSprite*)_fragsArray->objectAtIndex(i);
        maskSprite->setAnchorPoint(maskSprite->getTrueAnchorPoint());
        maskSprite->setPosition(maskSprite->getEndPoint());
        _showFragsLayer->addChild(maskSprite);
    }
}

//整张图片用1秒时间消失：
void PuzzleScene::displayPic()
{
	CCActionInterval *fadeOut = CCFadeOut::create(1);
	_pic->runAction(fadeOut);
    
}
//所有的切片显示出来：
void PuzzleScene::displayFrags()
{
    CCActionInterval *fadeIn = CCFadeIn::create(1);
    _showFragsLayer->runAction(fadeIn);
}





