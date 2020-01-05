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

        int state = 0;
        static const int STARTING_STATE = 0;
        static const int PLAYING_STATE = 1;
        static const int FINISHED_STATE = 2;

        const int ground_bit_mask_category = 0x0001;
        const int player_bit_mask_category = 0x0002;
        const int obstacle_bit_mask_category = 0x0004;

        const int player_tag = 1;
        const int ground_tag = 2;
        const int column_tag = 3;
        const int scoreArea_tag = 4;

        const char* player_name = "player";
        const char* ground_name = "ground";
        const char* column_name = "column";
        const char* scoreArea_name = "scoreArea";

        float jumpForce = 10000.0;

        float speedObstacle = 200.0f;

        float worldSpeed = 1.0;
        float gravitySpeed = worldSpeed*3; //3X world

        float heightLimityGame = 512;

};
#endif
