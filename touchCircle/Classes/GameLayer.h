#ifndef __GAMELAYER_SCENE_H__
#define __GAMELAYER_SCENE_H__

#define SCALE_POINT(__POINT) (cocos2d::CCE)
#define KUROISHI "kuro.png"
#define SHIROISHI "shiro.png"

#include "cocos2d.h"
#include "gameSpace.h"


class GameLayer: public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);

    cocos2d::CCEGLView* pEGLView = cocos2d::CCEGLView::sharedOpenGLView();
    float scaleSize = 320 / pEGLView->getDesignResolutionSize().width;
    
    //石を置いた数
    int countNum;
    
    //マスの情報
    gameSpace boardNum[6][6];
    
    // タッチイベント用関数の宣言
    bool ccTouchBegan(cocos2d::CCTouch* ptouch, cocos2d::CCEvent* pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void checkStone(int positionX, int positionY);
    void setStone(int positionX, int positionY);
    void swapStone(int positionX, int positionY, int color);
    cocos2d::CCPoint getStonePosition(int positionX, int positionY);
    cocos2d::CCPoint getStonePositionPx(cocos2d::CCSprite* sprite);
    void swapSprite(int tag, cocos2d::CCSprite* sprite);
    bool reverseCheck(int positionX, int positionY, int directionX, int directionY, int color);
    bool reverseColor(int positionX, int positionY, int directionX, int directionY, int color);
};

#endif // __GAMELAYER_SCENE_H__
