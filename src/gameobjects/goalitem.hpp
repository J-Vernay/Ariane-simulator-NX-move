#ifndef GOALITEM_HPP
#define GOALITEM_HPP

#include "abstractitem.hpp"

class GoalItem : public AbstractItem
{
public:
    GoalItem(double xpos, double ypos, double radius = 0.1, QString texturePath = QString(":/res/textures/logoTSE.png"));
};

#endif // GOALITEM_HPP
