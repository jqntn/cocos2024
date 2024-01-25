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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Main : public cocos2d::Layer
{
public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(Main);

  void initAudio();
  void initTouch();
  void initPhysics();
  void initMuteButton();
  void initBackButtonListener();

  void addBombs(float deltaTime);
  void removeBombs(float deltaTime);
  void updateScore(float deltaTime);

  void muteCallback(cocos2d::Ref* pSender) const;
  void pauseCallback(cocos2d::Ref* pSender) const;
  void menuCloseCallback(cocos2d::Ref* pSender) const;

  void setPhysicsBody(cocos2d::Sprite* sprite,
                      const cocos2d::Vec2& offset = cocos2d::Vec2::ZERO) const;
  void movePlayerByTouch(cocos2d::Touch* touch, cocos2d::Event* event) const;
  void movePlayerIfPossible(float newX) const;
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event* event) const;

  bool onCollision(cocos2d::PhysicsContact& contact) const;
  bool explodeBombs(cocos2d::Touch* touch, cocos2d::Event* event);

  int _score = 0;
  int _musicId = 0;
  cocos2d::Sprite* _bombSprite = nullptr;
  cocos2d::Sprite* _playerSprite = nullptr;
  cocos2d::MenuItemImage* _muteItem = nullptr;
  cocos2d::MenuItemImage* _unmuteItem = nullptr;
  cocos2d::Vector<cocos2d::Sprite*> _bombs;
};

#endif // __HELLOWORLD_SCENE_H__