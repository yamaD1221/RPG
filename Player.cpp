#include "player.h"

void Player::TakeDamage(int damage)
{
    data.HP -= damage;

    if (data.HP < 0)
        data.HP = 0;
}

bool Player::IsAlive() const
{
    return data.HP > 0;
}