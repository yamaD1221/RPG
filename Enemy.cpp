#include "Enemy.h"

void Enemy::TakeDamage(int damage)
{
    data.HP -= damage;

    if (data.HP < 0)
        data.HP = 0;
}

bool Enemy::IsAlive() const
{
    return data.HP > 0;
}