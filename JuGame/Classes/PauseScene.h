#ifndef __Pause_SCENE_H__
#define __Pause_SCENE_H__

#include "cocos2d.h"

class Pause : public cocos2d::Layer
{
public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(Pause);

  void exitPause(cocos2d::Ref* pSender) const;
};

#endif // __Pause_SCENE_H__