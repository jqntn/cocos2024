#ifndef __GameOver_SCENE_H__
#define __GameOver_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(GameOver);

  void exitPause(cocos2d::Ref* pSender) const;
};

#endif // __GameOver_SCENE_H__