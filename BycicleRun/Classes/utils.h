#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

USING_NS_CC;

// ����ģʽ
extern ResolutionPolicy gSolutionType;
// ʵ�ʷֱ���
extern CCSize gViewSize;
// ��Ʒֱ���
extern CCSize gDesignSize;

float transXPoint(float x);

float transYPoint(float y);

CCPoint transPoint(CCPoint p);

#endif