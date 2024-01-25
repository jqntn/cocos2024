#include "GameOverScene.h"
#include "MainScene.h"
#include <fstream>

USING_NS_CC;

static constexpr auto HIGHSCORE_PATH = "./highscore.txt";

Scene*
GameOver::createScene()
{
  auto scene = Scene::create();
  auto layer = GameOver::create();
  scene->addChild(layer);
  return scene;
}

bool
GameOver::init()
{
  if (!Layer::init()) {
    return false;
  }

  const auto size = Director::getInstance()->getVisibleSize();
  const auto origin = Director::getInstance()->getVisibleOrigin();

  const auto pauseCallback = CC_CALLBACK_1(GameOver::exitPause, this);
  const auto pauseItem =
    MenuItemImage::create("play.png", "play_pressed.png", pauseCallback);
  pauseItem->setPosition(
    Vec2(origin.x + size.width - pauseItem->getContentSize().width / 2,
         origin.y + pauseItem->getContentSize().height / 2));

  const auto menu = Menu::create(pauseItem, nullptr);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  const auto background = Sprite::create("background.png");
  background->setAnchorPoint(Vec2::ZERO);
  background->setPosition(Vec2::ZERO);
  this->addChild(background, -1);

  const auto label =
    Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 96);
  label->enableOutline(Color4B(255, 0, 0, 100), 6);
  label->enableGlow(Color4B(255, 0, 0, 255));
  label->enableShadow();
  label->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
  this->addChild(label, 1);

  /* Highscore Management */
  auto score = 0;
  auto highscore = 0;
  {
    score = UserDefault::getInstance()->getIntegerForKey("score", 0);

    /* Retrieve Highscore */
    auto inFile = std::ifstream(HIGHSCORE_PATH);
    if (inFile.good()) {
      highscore =
        std::stoi(std::string((std::istreambuf_iterator<char>(inFile)),
                              (std::istreambuf_iterator<char>())));
    }

    if (score > highscore) {
      highscore = score;
    }

    /* Save Highscore */
    auto outFile = std::ofstream(HIGHSCORE_PATH);
    outFile << highscore;
    outFile.close();
  }

  const auto labelHighscore0 =
    Label::createWithSystemFont("Your highscore is:", "Arial", 48);
  labelHighscore0->setPosition(origin.x + size.width / 2,
                               origin.y + size.height / 1.4f);
  this->addChild(labelHighscore0, 1);

  const auto labelHighscore1 =
    Label::createWithSystemFont(std::to_string(highscore), "Arial", 48);
  labelHighscore1->setPosition(origin.x + size.width / 2,
                               origin.y + size.height / 1.55f);
  labelHighscore1->setTextColor(Color4B(255, 255, 0, 255));
  this->addChild(labelHighscore1, 1);

  const auto labelScore0 =
    Label::createWithSystemFont("Your score is:", "Arial", 48);
  labelScore0->setPosition(origin.x + size.width / 2,
                           origin.y + size.height / 2.5f);
  this->addChild(labelScore0, 1);

  const auto labelScore1 =
    Label::createWithSystemFont(std::to_string(score), "Arial", 48);
  labelScore1->setPosition(origin.x + size.width / 2,
                           origin.y + size.height / 3);
  labelScore1->setTextColor(Color4B(255, 0, 255, 255));
  this->addChild(labelScore1, 1);

  return true;
}

void
GameOver::exitPause(Ref* pSender) const
{
  Director::getInstance()->replaceScene(
    TransitionFlipY::create(.5f, Main::createScene()));
}