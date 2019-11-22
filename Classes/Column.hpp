//
//  Column.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef Column_hpp
#define Column_hpp

#include "cocos2d.h"
using namespace cocos2d;

class Column : public Sprite
{
    public:
        Column();
        ~Column();
        static Column* create();
};

#endif 