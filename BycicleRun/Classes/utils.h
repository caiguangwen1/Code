#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

USING_NS_CC;

// 适配模式
extern ResolutionPolicy gSolutionType;
// 实际分辨率
extern CCSize gViewSize;
// 设计分辨率
extern CCSize gDesignSize;

float transXPoint(float x);

float transYPoint(float y);

CCPoint transPoint(CCPoint p);

#endif