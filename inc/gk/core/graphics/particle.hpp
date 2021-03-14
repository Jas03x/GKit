#ifndef GK_PARTICLE_H
#define GK_PARTICLE_H

typedef struct Particle
{
    float			position[3];
    unsigned char	uv_coord[2];
    unsigned short	size;
} Particle;

#endif // GK_PARTICLE