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

#include "HelloWorldScene.h"

USING_NS_CC;

static void
problemLoading(const char* filename)
{
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in "
         "front of filenames in HelloWorldScene.cpp\n");
}

Scene*
HelloWorld::createScene()
{
  return HelloWorld::create();
}

bool
HelloWorld::init()
{
  if (!Scene::init()) {
    return false;
  }

  const auto visibleSize = Director::getInstance()->getVisibleSize();
  const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

  const auto closeItem =
    MenuItemImage::create("CloseNormal.png",
                          "CloseSelected.png",
                          CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

  if (nullptr == closeItem || closeItem->getContentSize().width <= 0 ||
      closeItem->getContentSize().height <= 0) {
    problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
  } else {
    const auto x = visibleOrigin.x + visibleSize.width -
                   closeItem->getContentSize().width / 2;
    const auto y = visibleOrigin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Vec2(x, y));
  }

  const auto menu = Menu::create(closeItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
  if (nullptr == label) {
    problemLoading("'fonts/Marker Felt.ttf'");
  } else {
    label->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2,
                            visibleOrigin.y + visibleSize.height -
                              label->getContentSize().height));
    this->addChild(label, 1);
  }

  const auto sprite = Sprite::create("HelloWorld.png");
  if (nullptr == sprite) {
    problemLoading("'HelloWorld.png'");
  } else {
    sprite->setPosition(Vec2(visibleSize.width / 2 + visibleOrigin.x,
                             visibleSize.height / 2 + visibleOrigin.y));
    this->addChild(sprite, 0);
  }
  return true;
}

void
HelloWorld::menuCloseCallback(Ref* pSender)
{
  Director::getInstance()->end();

  /*To navigate back to native iOS screen (if present) without quitting the
   * application, do not use Director::getInstance()->end() as given
   * above, instead trigger a custom event created in RootViewController.mm as
   * below*/

  // EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);
}