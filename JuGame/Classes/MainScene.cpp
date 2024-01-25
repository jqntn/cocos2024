/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"
#include <AudioEngine.h>

USING_NS_CC;

static void
problemLoading(const char* filename)
{
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in "
         "front of filenames in HelloWorldScene.cpp\n");
}

Scene*
Main::createScene()
{
  const auto scene = Scene::createWithPhysics();

  scene->addChild(Main::create());

  scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
#if COCOS2D_DEBUG
  scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif

  return scene;
}

bool
Main::init()
{
  if (!Layer::init()) {
    return false;
  }

  const auto size = Director::getInstance()->getVisibleSize();
  const auto origin = Director::getInstance()->getVisibleOrigin();

  /* Pause Button */
  const auto pauseCallback = CC_CALLBACK_1(Main::pauseCallback, this);
  const auto pauseItem =
    MenuItemImage::create("pause.png", "pause_pressed.png", pauseCallback);
  pauseItem->setPosition(
    Vec2(origin.x + size.width - pauseItem->getContentSize().width / 2,
         origin.y + pauseItem->getContentSize().height / 2));

  /* Menu */
  const auto menu = Menu::create(pauseItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  /* Background Sprite */
  const auto background = Sprite::create("background.png");
  background->setAnchorPoint(Vec2::ZERO);
  background->setPosition(Vec2::ZERO);
  this->addChild(background, -1);

  /* Player Sprite */
  const auto playerSheet = "character_zombie_sheet.png";
  const auto playerRect0 = Rect(0, 0, 864 / 9.0f, 640 / 5.0f);
  const auto playerRect1 = Rect((864 / 9.0f) * 7, 0, 864 / 9.0f, 640 / 5.0f);

  _playerSprite = Sprite::create(playerSheet, playerRect0);
  _playerSprite->setPosition(size.width / 2, size.height * .22f);
  this->addChild(_playerSprite, 0);
  setPhysicsBody(_playerSprite, Vec2(0, -20));

  /* Bomb Sprite */
  _bombSprite = Sprite::create("bomb.png");
  _bombSprite->setPosition(
    size.width / 2, size.height + _bombSprite->getContentSize().height / 2);
  this->addChild(_bombSprite, 1);
  setPhysicsBody(_bombSprite, Vec2(-3, 3));

  _bombSprite->getPhysicsBody()->setVelocity(Vec2(0, -100));
  _bombs.pushBack(_bombSprite);

  /* Animations */
  auto frames = Vector<SpriteFrame*>();
  frames.pushBack(SpriteFrame::create(playerSheet, playerRect0));
  frames.pushBack(SpriteFrame::create(playerSheet, playerRect1));

  const auto animation = Animation::createWithSpriteFrames(frames, .2f);
  const auto animator = Animate::create(animation);

  _playerSprite->runAction(RepeatForever::create(animator));

  initAudio();
  initTouch();
  initPhysics();
  initMuteButton();
  initBackButtonListener();

  schedule(CC_SCHEDULE_SELECTOR(Main::updateScore), 2);
  schedule(CC_SCHEDULE_SELECTOR(Main::addBombs), 8);
  schedule(CC_SCHEDULE_SELECTOR(Main::removeBombs), 2);

  return true;
}

void
Main::initAudio()
{
  if (!AudioEngine::lazyInit()) {
    CCLOG("Error while initializing audio engine");
    return;
  }
  CCLOG("Audio engine initialized successfully");

  _musicId = AudioEngine::play2d("music.mp3");
  AudioEngine::setVolume(_musicId, 1);
  AudioEngine::setLoop(_musicId, true);
}

void
Main::initTouch()
{
  const auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2(Main::explodeBombs, this);
  listener->onTouchMoved = CC_CALLBACK_2(Main::movePlayerByTouch, this);
  listener->onTouchEnded = [=](Touch* touch, Event* event) {};
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void
Main::initPhysics()
{
  const auto listener = EventListenerPhysicsContact::create();
  listener->onContactBegin = CC_CALLBACK_1(Main::onCollision, this);
  getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void
Main::initMuteButton()
{
  const auto size = Director::getInstance()->getVisibleSize();

  _muteItem = MenuItemImage::create(
    "mute.png", "mute.png", CC_CALLBACK_1(Main::muteCallback, this));
  _unmuteItem = MenuItemImage::create(
    "unmute.png", "unmute.png", CC_CALLBACK_1(Main::muteCallback, this));

  _muteItem->setPosition(
    Vec2(size.width - _muteItem->getContentSize().width / 2,
         size.height - _muteItem->getContentSize().height / 2));
  _unmuteItem->setPosition(
    Vec2(size.width - _unmuteItem->getContentSize().width / 2,
         size.height - _unmuteItem->getContentSize().height / 2));

  const auto menu = Menu::create(_muteItem, _unmuteItem, nullptr);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  _unmuteItem->setVisible(false);
}

void
Main::initBackButtonListener()
{
  const auto listener = EventListenerKeyboard::create();
  listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {};
  listener->onKeyReleased = CC_CALLBACK_2(Main::onKeyPressed, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void
Main::addBombs(float deltaTime)
{
  const auto size = Director::getInstance()->getVisibleSize();

  for (auto i = 0; i < 3; i++) {
    const auto bomb = Sprite::create("bomb.png");
    bomb->setPosition(CCRANDOM_0_1() * size.width,
                      size.height + bomb->getContentSize().height / 2);
    this->addChild(bomb, 1);
    setPhysicsBody(bomb, Vec2(-3, 3));
    bomb->getPhysicsBody()->setVelocity(
      Vec2(0, ((CCRANDOM_0_1() + .2f) * -250)));
    _bombs.pushBack(bomb);
  }
}

void
Main::removeBombs(float deltaTime)
{
  for (const auto bomb : _bombs) {
    if (bomb->getPositionY() < 0) {
      this->removeChild(bomb);
    }
  }
}

void
Main::updateScore(float deltaTime)
{
  _score += 10;
}

void
Main::muteCallback(Ref* pSender) const
{
  AudioEngine::setVolume(_musicId, !_muteItem->isVisible());

  _muteItem->setVisible(!_muteItem->isVisible());
  _unmuteItem->setVisible(!_muteItem->isVisible());
}

void
Main::pauseCallback(Ref* pSender) const
{
  _director->pushScene(Pause::createScene());
}

void
Main::menuCloseCallback(Ref* pSender) const
{
  Director::getInstance()->end();

  /*To navigate back to native iOS screen (if present) without quitting the
   * application, do not use Director::getInstance()->end() as given
   * above, instead trigger a custom event created in RootViewController.mm as
   * below*/

  // EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void
Main::setPhysicsBody(Sprite* sprite, const Vec2& offset) const
{
  const auto body = PhysicsBody::createCircle(
    sprite->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT, offset);
  body->setContactTestBitmask(true);
  body->setDynamic(true);
  sprite->setPhysicsBody(body);
}

void
Main::movePlayerByTouch(Touch* touch, Event* event) const
{
  const auto touchLocation = touch->getLocation();
  const auto playerShape = _playerSprite->getPhysicsBody()->getFirstShape();

  if (playerShape->containsPoint(touchLocation)) {
    movePlayerIfPossible(touchLocation.x);
  }
}

void
Main::movePlayerIfPossible(float newX) const
{
  const auto size = Director::getInstance()->getVisibleSize();
  const auto playerSpriteHalfWidth =
    _playerSprite->getBoundingBox().size.width / 2;

  if (newX >= playerSpriteHalfWidth &&
      newX < size.width - playerSpriteHalfWidth) {
    _playerSprite->setPositionX(newX);
  }
}

void
Main::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) const
{
  if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
    Director::getInstance()->end();
  }
}

bool
Main::onCollision(PhysicsContact& contact) const
{
  CCLOG("Collision detected");

  const auto playerShape = _playerSprite->getPhysicsBody()->getFirstShape();
  if (playerShape != contact.getShapeA() &&
      playerShape != contact.getShapeB()) {
    return false;
  }

  if (_muteItem->isVisible()) {
    AudioEngine::stopAll();
    AudioEngine::play2d("uh.mp3");
  }

  UserDefault::getInstance()->setIntegerForKey("score", _score);
  _director->replaceScene(
    TransitionFlipY::create(.5f, GameOver::createScene()));

  return false;
}

bool
Main::explodeBombs(Touch* touch, Event* event)
{
  const auto touchLocation = touch->getLocation();
  auto trash = Vector<Sprite*>();

  for (const auto bomb : _bombs) {
    if (bomb->getPhysicsBody()->getFirstShape()->containsPoint(touchLocation)) {
      AudioEngine::play2d("bomb.mp3");
      const auto explosion = ParticleExplosion::create();
      explosion->setPosition(bomb->getPosition());
      explosion->setTotalParticles(1000);
      explosion->setSpeed(10);
      explosion->setLife(1);
      this->addChild(explosion);
      bomb->setVisible(false);
      this->removeChild(bomb);
      trash.pushBack(bomb);
    }
  }

  for (const auto bomb : trash) {
    _bombs.eraseObject(bomb);
  }

  return true;
}