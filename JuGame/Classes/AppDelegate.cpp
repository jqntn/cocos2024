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

#define USE_AUDIO_ENGINE 1

#include "AppDelegate.h"
#include "MainScene.h"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
// using namespace  experimental;
#endif

USING_NS_CC;

static constexpr auto GAME_NAME = "JuGame";

static const auto designResolutionSize = Size(480, 768);
static const auto smallResolutionSize = Size(480, 320);
static const auto mediumResolutionSize = Size(1024, 768);
static const auto largeResolutionSize = Size(2048, 1536);

static int
register_all_packages()
{
  return 0;
}

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
  AudioEngine::end();
#endif
}

void
AppDelegate::initGLContextAttrs()
{
  // red,green,blue,alpha,depth,stencil,multisamplesCount
  GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 4 };

  GLView::setGLContextAttrs(glContextAttrs);
}

bool
AppDelegate::applicationDidFinishLaunching()
{
  const auto director = Director::getInstance();

  auto glview = director->getOpenGLView();

  if (nullptr == glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) ||                               \
  (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||                                   \
  (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    const auto& size = designResolutionSize;
    const auto rect = Rect(0, 0, size.width, size.height);

    glview = GLViewImpl::createWithRect(GAME_NAME, rect);
#else
    glview = GLViewImpl::create(GAME_NAME);
#endif

    director->setOpenGLView(glview);
  }

#if COCOS2D_DEBUG
  director->setDisplayStats(true);
#endif
  director->setAnimationInterval(director->getFrameRate());

  glview->setDesignResolutionSize(designResolutionSize.width,
                                  designResolutionSize.height,
                                  ResolutionPolicy::NO_BORDER);

  const auto frameSize = glview->getFrameSize();
  if (frameSize.height > mediumResolutionSize.height) {
    director->setContentScaleFactor(
      MIN(largeResolutionSize.height / designResolutionSize.height,
          largeResolutionSize.width / designResolutionSize.width));
  } else if (frameSize.height > smallResolutionSize.height) {
    director->setContentScaleFactor(
      MIN(mediumResolutionSize.height / designResolutionSize.height,
          mediumResolutionSize.width / designResolutionSize.width));
  } else {
    director->setContentScaleFactor(
      MIN(smallResolutionSize.height / designResolutionSize.height,
          smallResolutionSize.width / designResolutionSize.width));
  }

  auto searchPaths = std::vector<std::string>();
  searchPaths.push_back("images");
  searchPaths.push_back("sounds");
  searchPaths.push_back("particles");
  FileUtils::getInstance()->setSearchPaths(searchPaths);

  register_all_packages();

  director->runWithScene(Main::createScene());

  return true;
}

void
AppDelegate::applicationDidEnterBackground()
{
  Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
  AudioEngine::pauseAll();
#endif
}

void
AppDelegate::applicationWillEnterForeground()
{
  Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
  AudioEngine::resumeAll();
#endif
}