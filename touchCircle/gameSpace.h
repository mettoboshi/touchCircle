//
//  gameSpace.h
//  touchCircle
//
//  Created by FUJITA YASUO on 2013/08/17.
//
//

#ifndef __touchCircle__gameSpace__
#define __touchCircle__gameSpace__

#include <iostream>

class gameSpace
{
public:
    int color; // Nothing : 0, black : 1 , white : 2
    int tagNo;  // 初期値 : -1

    bool init();
    
};


#endif /* defined(__touchCircle__gameSpace__) */

