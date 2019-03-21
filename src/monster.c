#include "monster.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"
//WIP (Should return an entity) (MonsterType Determines what think fucntion is assigned!)
Entity *monster_spawn(Vector2D location,MonsterType type){

	Entity *self;
	self = entity_new();
	self->position = location;
	self->velocity = vector2d(0, 0);
	if (!self)return NULL;

	self->shape = shape_rect(32,32, 64, 64);
	body_set(
		&self->hitbox,
		"monster",
		1,//world layer
		1,
		0,
		1,
		location,
		vector2d(0, 0),
		10,
		1,
		0,
		&self->shape,
		self,
		body_body_touch);
	//set functions to function pointers here
	switch (type)
	{
	case PATROLLER:
		self->think = patroller_think;
		break;
	case CHASER:
		self->think = chaser_think;
		break;
	case SHOOTER:
		self->think = shooter_think;
		break;
	}

	return self;
	
}

void monster_update(Entity *ent){
	ent->frame += 0.1;
	if (ent->frame >= 16.0)ent->frame = 0;


}
void shooter_think(Entity *ent){
	//slog("IM A SHOOTER");
	Entity *sprite;
	ent->velocity = vector2d(0, 0);
	if (ent->timer % 100 == 0){
		sprite = entity_projectile(ent, vector2d(0, -1));
		sprite->sprite = gf2d_sprite_load_all("images/Fireball.png", 32, 32, 16);
	}

}
void patroller_think(Entity *ent){
	//slog("IM A PATROLLER");
	//slog("%d",ent->timer);
	if (ent->timer % 100 == 0){
		ent->velocity = vector2d(-ent->velocity.x, -ent->velocity.y);
	}
}
void chaser_think(Entity *ent){
	slog("IM A CHASER");
	Vector2D chase;
	double outX;
	double outY;
	if (monster_player_sight_check(ent)){
		outX = ent->position.x - player_get()->position.x;
		outY = ent->position.y - player_get()->position.y;
		chase = vector2d(outX, outY);
		double mag = vector2d_magnitude(chase);
		outX = outX / mag;
		outY = outY / mag;
		chase = vector2d(-outX, -outY);
		ent->velocity = chase;
	} else{
	ent->velocity = vector2d(0, 0);
	}
}
int monster_player_sight_check(Entity *self)
{
	Entity *player = player_get();
	if (!player)return 0;
	if (vector2d_magnitude_compare(vector2d(self->position.x - player->position.x, self->position.y - player->position.y), 200) < 0)
	{
		
		return 1;
	}
	return 0;
}