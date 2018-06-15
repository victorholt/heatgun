#ifndef OPENSIMPLEX_NOISE_H
#define OPENSIMPLEX_NOISE_H

#include <core/Godot.hpp>
#include <Reference.hpp>
#include "lib/open-simplex-noise.h"

using namespace godot;

// C++ wrapper for OpenSimplex context.
// Allocates permutation memory for each instance.
class OsnNoise : public GodotScript<Reference> {
    GODOT_CLASS(OsnNoise)

    osn_context * _context;
    int _seed;

public:
    OsnNoise();
    ~OsnNoise();

    void set_seed(int seed);
    int get_seed();

    inline float get_noise_2d(float x, float y) { return open_simplex_noise2(_context, x, y); }
    inline float get_noise_3d(float x, float y, float z) { return open_simplex_noise3(_context, x, y, z); }
    inline float get_noise_4d(float x, float y, float z, float w) { return open_simplex_noise4(_context, x, y, z, w); }

    // Convenience

    inline float get_noise_2dv(Vector2 v) { return open_simplex_noise2(_context, v.x, v.y); }
    inline float get_noise_3dv(Vector3 v) { return open_simplex_noise3(_context, v.x, v.y, v.z); }

    static void _register_methods();

};

// TODO: if the OpenSimplex library didn't allocated memory for each seed, there could be just one, easy to use class...

// Fractal noise generator with a few tweakable parameters. Lightweight.
// This is mostly used in games for generating heightmaps or clouds.
class OsnFractalNoise : public GodotScript<Reference> {
    GODOT_CLASS(OsnFractalNoise)

    OsnNoise* _noise;

    float _persistance; // Controls details, value in [0,1]. Higher increases grain, lower increases smoothness.
    int _octaves; // Number of noise layers
    float _period; // Distance above which we start to see similarities. The higher, the longer "hills" will be on a terrain.
    float _lacunarity; // Controls period change across octaves. 2 is usually a good value to address all detail levels.

public:
    OsnFractalNoise();

    OsnNoise* get_source_noise() { return _noise; }
    void set_source_noise(OsnNoise* noise) { _noise = noise; }

    void set_octaves(int count) { _octaves = count; }
    int get_octaves() const { return _octaves; }

    void set_period(float p) { _period = p; }
    float get_period() const { return _period; }

    void set_persistance(float p) { _persistance = p; }
    float get_persistance() const { return _persistance; }

    void set_lacunarity(float lacunarity) { _lacunarity = lacunarity; }
    float get_lacunarity() const { return _lacunarity; }

    float get_noise_2d(float x, float y);
    float get_noise_3d(float x, float y, float z);
    float get_noise_4d(float x, float y, float z, float w);

    // Convenience

    inline float get_noise_2dv(Vector2 v) { return get_noise_2d(v.x, v.y); }
    inline float get_noise_3dv(Vector3 v) { return get_noise_3d(v.x, v.y, v.z); }

    static void _register_methods();

};

/** GDNative Initialize **/
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    godot::Godot::gdnative_init(o);
}

/** GDNative Terminate **/
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
    godot::Godot::gdnative_terminate(o);
}

/** NativeScript Initialize **/
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
    godot::Godot::nativescript_init(handle);

    godot::register_class<OsnNoise>();
    godot::register_class<OsnFractalNoise>();
}

#endif // OPENSIMPLEX_NOISE_H


