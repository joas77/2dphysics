#include "Contact.h"

void Contact::ResolvePenetration()
{
    if(a->IsStatic() && b->IsStatic()) return;
    
    auto da = depth / (a->GetInvMass() + b->GetInvMass()) * a->GetInvMass();
    auto db = depth / (a->GetInvMass() + b->GetInvMass()) * b->GetInvMass();

    a->position -= normal * da;
    b->position += normal * db;
}