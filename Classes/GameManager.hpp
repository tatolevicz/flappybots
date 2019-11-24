//
//  GameManager.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
class GameManager
{
    private:
        static GameManager* instance;    

    public:
        GameManager();
        ~GameManager();
        static GameManager* getInstance();
        const int ground_bit_mask_category = 0x01;
        const int player_bit_mask_category = 0x02;
        const int obstacle_bit_mask_category = 0x03;

        const int player_tag = 1;
        const int ground_tag = 2;
        const int column_tag = 3;

        const char* player_name = "player";
        const char* ground_name = "ground";
        const char* column_name = "column";

        float gravitySpeed = 3.0;
        float jumpForce = 10000.0;
};
#endif
