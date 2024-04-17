#ifndef CONTACT_H
#define CONTACT_H

#include "Vec2.h"
#include "Body.h"

struct Contact
{
    const Body* a;
    const Body* b;

    Vec2 normal;
    Vec2 start;
    Vec2 end;

    float depth;

    void ResolvePenetration();
};


#endif // CONTACT_H