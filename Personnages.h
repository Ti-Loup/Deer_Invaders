
//Les classes Player et Deers herite de Personnages

#ifndef DEER_INVADERS_PERSONNAGES_H
#define DEER_INVADERS_PERSONNAGES_H
#include "Entity.h"


class Personnages {

};

class Enemy_Deer : public Entity{
public:
    Enemy_Deer(float startX, float startY);
};
class Enemy_HealerDeer : public Entity{
public:
    Enemy_HealerDeer(float startX, float startY);
};

#endif //DEER_INVADERS_PERSONNAGES_H
