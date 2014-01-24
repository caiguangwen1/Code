#include "utils.h"

ResolutionPolicy gSolutionType;
CCSize gViewSize;
CCSize gDesignSize;

// ת��x����
// 0 <= x <= DISIGN_SCREEN_WIDTH
float transXPoint(float x)
{
	if (gSolutionType == kResolutionFixedHeight)
	{
		x = x + (gViewSize.width - gDesignSize.width)/2;
	}

	return x;
}

// ת��y����
// 0 <= y <= DISIGN_SCREEN_HRIGHT
float transYPoint(float y)
{
	if (gSolutionType == kResolutionFixedWidth)
	{
		y = y + (gViewSize.height - gDesignSize.height)/2;
	}

	return y;
}

// ת��������
// 0 <= x <= DISIGN_SCREEN_WIDTH
// 0 <= y <= DISIGN_SCREEN_HEIGHT
CCPoint transPoint(CCPoint p)
{
	if (gSolutionType == kResolutionFixedWidth)
	{
		p.y += (gViewSize.height - gDesignSize.height)/2;
	} 
	else
	{
		p.x += (gViewSize.width - gDesignSize.width)/2;
	}

	return p;
}