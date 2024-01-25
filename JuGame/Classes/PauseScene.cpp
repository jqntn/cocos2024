#include "PauseScene.h"

USING_NS_CC;

Scene*
Pause::createScene()
{
  auto scene = Scene::create();
  auto layer = Pause::create();
  scene->addChild(layer);
  return scene;
}

bool
Pause::init()
{
  if (!Layer::init()) {
    return false;
  }

  const auto size = Director::getInstance()->getVisibleSize();
  const auto origin = Director::getInstance()->getVisibleOrigin();

  const auto pauseCallback = CC_CALLBACK_1(Pause::exitPause, this);
  const auto pauseItem =
    MenuItemImage::create("play.png", "play_pressed.png", pauseCallback);
  pauseItem->setPosition(
    Vec2(origin.x + size.width - pauseItem->getContentSize().width / 2,
         origin.y + pauseItem->getContentSize().height / 2));

  const auto menu = Menu::create(pauseItem, nullptr);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  const auto label = Label::createWithTTF("PAUSE", "fonts/Marker Felt.ttf", 96);
  label->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
  this->addChild(label, 1);

  return true;
}

void
Pause::exitPause(Ref* pSender) const
{
  Director::getInstance()->popScene();
}