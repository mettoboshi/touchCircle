#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCLayerColor *color = CCLayerColor::create(ccc4(100.0f,100.0f,100.0f,255.0f));
    this->addChild(color);
    
    CCSize s = this->getContentSize();
    CCLOG("size w=%f, h=%f", s.width, s.height);
    
    // スプライトの作成
    CCSprite* spriteBoard = CCSprite::create("koushi.png");
    
    //画像の座標の決定
    CCLOG("scalesize : %f",scaleSize);
    
    spriteBoard->setPosition(ccp(160 / scaleSize, (160 / scaleSize)));
    
    //画像をレイヤに追加
    this->addChild(spriteBoard);
 
    // オセロ板のデータを初期化
    countNum = 0;
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            boardNum[i][j].tagNo = -1;
        }
    }
    
    //　石の初期配置
    this->setStone(2, 2);
    this->setStone(2, 3);
    this->setStone(3, 3);
    this->setStone(3, 2);
    
    // タッチモードを設定する
    this->setTouchMode(kCCTouchesOneByOne);
    
    // タッチを有効にする
    this->setTouchEnabled(true);

    
    return true;
}

// タッチ用メソッド
bool GameLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    //ゲーム終了
    CCLOG("countnum = %d",countNum);
    if(countNum >= 37) {
        for(int i=0; i<36; i++) {
            this->removeChildByTag(i);
        }
        return true;
    }
    
    //タッチした座標を取得
    CCPoint location =pTouch->getLocation();
    
    int positionX = floor((location.x - 10) * scaleSize / 50);
    int positionY = floor((location.y - 10) * scaleSize / 50);
    
    // 範囲外に石を置けないようにする
    if ((positionX < 0 || positionX > 5) || (positionY < 0 || positionY > 5)) {
        return false;
    }
    
    this->checkStone(positionX, positionY);
    
    return true;
}

void GameLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void GameLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void GameLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void GameLayer::setStone(int positionX, int positionY) {

    //石の配置
    CCSprite* stone;
    if (countNum % 2 == 0) {
        stone = CCSprite::create(KUROISHI);
    } else {
        stone = CCSprite::create(SHIROISHI);
    }
    
    stone->setPosition(this->getStonePosition(positionX, positionY));
    stone->setTag(countNum);
    this->addChild(stone);
    
    //石の場所を格納
    boardNum[positionY][positionX].tagNo = countNum;
    boardNum[positionY][positionX].color = (countNum % 2) + 1;
    
    countNum++;
    
    return;
}


void GameLayer::swapStone(int positionX, int positionY, int color) {

    //不要な石を除去
    this->removeChildByTag(boardNum[positionY][positionX].tagNo);
    
    //石の配置
    CCSprite* stone;
    if (color % 2 != 0) {
        stone = CCSprite::create(KUROISHI);
    } else {
        stone = CCSprite::create(SHIROISHI);
    }
    
    stone->setPosition(this->getStonePosition(positionX, positionY));
    stone->setTag(boardNum[positionY][positionX].tagNo);
    this->addChild(stone);
    
    //石の場所を格納
    boardNum[positionY][positionX].color = color;
    
    return;
}


void GameLayer::checkStone(int positionX, int positionY) {

    bool flag=false;
    
    //同じ場所に石がないことを確認する
    if(boardNum[positionY][positionX].tagNo != -1) {
        return;
    }

    //反転させられる石があることを確認
    for(int i = -1; i <= 1; i++) {
        for (int j = -1; j <=1; j++) {
            if(i == 0 && j == 0) {
                continue;
            }
            if(this->reverseCheck(positionX, positionY, i, j, (countNum % 2) + 1)){
                flag = true;
                this->reverseColor(positionX, positionY, i, j, (countNum % 2) + 1);
            }
        }
    }

    if (flag != true) {
        return;
    }
    
    setStone(positionX, positionY);
    
    return;
}

CCPoint GameLayer::getStonePosition(int positionX, int positionY)
{
    //ポジションから座標を求め返戻
    return (ccp((positionX * 50 + 35) / scaleSize, (positionY * 50 + 35) / scaleSize));
}

CCPoint GameLayer::getStonePositionPx(CCSprite* sprite) {
    CCPoint position;
    position = sprite->getPosition();
    //スプライトの座標を返戻
    return (ccp(((position.x * scaleSize)- 35) / 50, ((position.y * scaleSize) - 35) / 50));
}

void GameLayer::swapSprite(int tag, CCSprite* sprite) {
    //スプライトの入れ替え
    this->removeChildByTag(tag);
    this->addChild(sprite);
    sprite->setTag(tag);
}

bool GameLayer::reverseCheck(int positionX, int positionY, int directionX, int directionY, int color) {
    
    int tmpX = positionX;
    int tmpY = positionY;
    int opponentColor = 3 - color;
    bool flag = false;
    tmpX += directionX;
    tmpY += directionY;
    
    if(boardNum[tmpY][tmpX].color != opponentColor) {
        return false;
    }
    
    while(tmpX >= 0 && tmpX <= 6 && tmpY >= 0 && tmpY <= 6) {
    
        if(boardNum[tmpY][tmpX].color == opponentColor) {
            flag = true;
        } else if ((boardNum[tmpY][tmpX].color == color) && (flag == true)) {
            return true;
        } else {
            return false;
        }
        
        tmpX += directionX;
        tmpY += directionY;
    }
    return false;
}

bool GameLayer::reverseColor(int positionX, int positionY, int directionX, int directionY, int color) {
    int opponentColor = 3 - color;
    int tmpX = positionX + directionX;
    int tmpY = positionY + directionY;

    while(tmpX >= 0 && tmpX <= 6 && tmpY >= 0 && tmpY <= 6) {
        if (boardNum[tmpY][tmpX].color == opponentColor) {
            this->swapStone(tmpX, tmpY, color);
        } else if (boardNum[tmpY][tmpX].color == color) {
            return true;
        }
        
        tmpX += directionX;
        tmpY += directionY;
    }
    
    return true;
}
