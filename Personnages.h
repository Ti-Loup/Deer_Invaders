
//Les classes Player et Deers herite de Personnages

#ifndef DEER_INVADERS_PERSONNAGES_H
#define DEER_INVADERS_PERSONNAGES_H
#include "Entity.h"


class Personnages {

};


//Les types de cerfs

//Cerfs generaux
class Enemy_Deer : public Entity{
public:
    Enemy_Deer(float startX, float startY);
};
//Les soigneurs
class Enemy_HealerDeer : public Entity{
public:
    Enemy_HealerDeer(float startX, float startY);
};



//Bosses
class Enemy_FraiseBoss : public Entity {
public:
    Enemy_FraiseBoss(float floatX, float floatY);

};

#endif //DEER_INVADERS_PERSONNAGES_H
