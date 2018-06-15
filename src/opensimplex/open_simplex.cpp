#include "open_simplex.h"

OsnNoise::OsnNoise() : _context(NULL), _seed(0) {
    open_simplex_noise(_seed, &_context);
}

OsnNoise::~OsnNoise() {
    if (_context) {
        open_simplex_noise_free(_context);
    }
}

void OsnNoise::set_seed(int seed) {
    if (seed == _seed)
        return;
    // This is not fair...
    if (_context)
        open_simplex_noise_free(_context);
    open_simplex_noise(seed, &_context);
    _seed = seed;
}

int OsnNoise::get_seed() {
    return _seed;
}

void OsnNoise::_register_methods() {

    register_method("get_seed", &OsnNoise::get_seed);
    register_method("set_seed", &OsnNoise::set_seed);

    register_method("get_noise_2d", &OsnNoise::get_noise_2d);
    register_method("get_noise_3d", &OsnNoise::get_noise_3d);
    register_method("get_noise_4d", &OsnNoise::get_noise_4d);

    register_method("get_noise_2dv", &OsnNoise::get_noise_2dv);
    register_method("get_noise_3dv", &OsnNoise::get_noise_3dv);

}


OsnFractalNoise::OsnFractalNoise() : _persistance(0.5), _octaves(3), _period(64), _lacunarity(2.0) {

}

float OsnFractalNoise::get_noise_2d(float x, float y) {
    OsnNoise & src = *_noise;

    x /= _period;
    y /= _period;

    float amp = 1.0;
    float max = 1.0;
    float sum = src.get_noise_2d(x, y);

    unsigned int i = 0;
    while (++i < _octaves) {
        x *= _lacunarity;
        y *= _lacunarity;
        amp *= _persistance;
        max += amp;
        // TODO usually, we increase the seed here on every loop to avoid similarities...
        // But that would mean allocating a new osn_context for each!
        // So I fallback on X modification.
        x += _period;
        sum += src.get_noise_2d(x, y) * amp;
    }

    return sum / max;
}

float OsnFractalNoise::get_noise_3d(float x, float y, float z) {
    OsnNoise & src = *_noise;

    x /= _period;
    y /= _period;
    z /= _period;

    float amp = 1.0;
    float max = 1.0;
    float sum = src.get_noise_3d(x, y, z);

    unsigned int i = 0;
    while (++i < _octaves) {
        x *= _lacunarity;
        y *= _lacunarity;
        z *= _lacunarity;
        amp *= _persistance;
        max += amp;
        x += _period;
        sum += src.get_noise_3d(x, y, z) * amp;
    }

    return sum / max;
}

float OsnFractalNoise::get_noise_4d(float x, float y, float z, float w) {
    OsnNoise & src = *_noise;

    x /= _period;
    y /= _period;
    z /= _period;
    w /= _period;

    float amp = 1.0;
    float max = 1.0;
    float sum = src.get_noise_4d(x, y, z, w);

    unsigned int i = 0;
    while (++i < _octaves) {
        x *= _lacunarity;
        y *= _lacunarity;
        z *= _lacunarity;
        w *= _lacunarity;
        amp *= _persistance;
        max += amp;
        x += _period;
        sum += src.get_noise_4d(x, y, z, w) * amp;
    }

    return sum / max;
}

void OsnFractalNoise::_register_methods() {

    register_method("get_source_noise", &OsnFractalNoise::get_source_noise);
    register_method("set_source_noise", &OsnFractalNoise::set_source_noise);

    register_method("set_octaves", &OsnFractalNoise::set_octaves);
    register_method("get_octaves", &OsnFractalNoise::get_octaves);

    register_method("set_period", &OsnFractalNoise::set_period);
    register_method("get_period", &OsnFractalNoise::get_period);

    register_method("set_persistance", &OsnFractalNoise::set_persistance);
    register_method("get_persistance", &OsnFractalNoise::get_persistance);

    register_method("set_lacunarity", &OsnFractalNoise::set_lacunarity);
    register_method("get_lacunarity", &OsnFractalNoise::get_lacunarity);

    register_method("get_noise_2d", &OsnFractalNoise::get_noise_2d);
    register_method("get_noise_3d", &OsnFractalNoise::get_noise_3d);
    register_method("get_noise_4d", &OsnFractalNoise::get_noise_4d);

    register_method("get_noise_2dv", &OsnFractalNoise::get_noise_2dv);
    register_method("get_noise_3dv", &OsnFractalNoise::get_noise_3dv);

}

