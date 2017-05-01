//
// Created by fbiedrzy on 5/28/2016.
//

#ifndef CORE_IOCALLBACKS_HPP
#define CORE_IOCALLBACKS_HPP


class IOCallbacks
{
public:
	static void drawRectangleOnClick(int event, int x, int y, int flags, void *userdata = 0);

	static void drawColourInfoOnClick(int event, int x, int y, int flags, void * userdata);

	static void drawLineOnClick(int evt, int x, int y, int flags, void *userdata);
};

#endif //CORE_IOCALLBACKS_HPP