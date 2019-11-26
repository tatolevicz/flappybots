//
//  AcademyFlappyBots.hpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 25/11/19.
//

#ifndef AcademyFlappyBots_hpp
#define AcademyFlappyBots_hpp

#include <stdio.h>
#include "AgentFlappyBot.hpp"

USING_NS_CC;

class AcademyFlappyBots{
    private:
        static AcademyFlappyBots* instance;
        void initialSetup();
        void initPool();
        void schedule();
        void unschedule();
        void update(float dt);
        // void tempCalc()
    public:
        AcademyFlappyBots();
        ~AcademyFlappyBots();
        static AcademyFlappyBots* getInstance();
        Vector<AgentFlappyBot*> *agentsPool = new Vector<AgentFlappyBot*>();
        int poolSize = 10;
};
#endif /* AcademyFlappyBots_hpp */
