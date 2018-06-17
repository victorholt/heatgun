/**********************************************************
 * Author:  Victor Holt
 * The MIT License (MIT)
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 **********************************************************/
#pragma once

#include <heatgun/defines.hpp>

#include <core/Godot.hpp>
#include <Reference.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>

class MapGenerator : public godot::GodotScript<godot::Reference>
{
    GODOT_CLASS(MapGenerator)

private:
    struct MapGenerationParams
    {
        // Seed for generating maps.
        long seed = 0;
        // The dungeon size.
        int map_size = 256;
        // Number of floors placed.
        int floors_placed = 0;
        // Size of a single floor tile.
        int floor_size = 8;
        // Max floors that can be placed.
        int max_floors = 500;
        // Current direction for the floor placement.
        int dir = 1;
        // Random starting x position.
        int rand_x = 0;
        // Random starting y position.
        int rand_y = 0;
        // Lower-bound position.
        godot::Vector2 lower_bound;
        // Upper-bound position.
        godot::Vector2 upper_bound;
        // Current x position.
        int current_x = 0;
        // Current y position.
        int current_y = 0;
        // Flag for whether or not the dungeon is generating.
        bool is_generating = false;
    };

    /// Reference to the map image.
    godot::Ref<godot::Image> map_image;

    /// Reference to the map texture.
    godot::Ref<godot::ImageTexture> map_texture;

    /// Reference to the detail map image.
    godot::Ref<godot::Image> detail_map_image;

    /// Reference to the detail map texture.
    godot::Ref<godot::Image> detail_map_texture;

    /// Map generation parameters.
    MapGenerationParams params;

    //! Builds out the floors.
    void _build_floors();

    //! Places a single floor.
    void _place_floor();

    //! Checks and updates the bounds if necessary.
    void _update_bounds();

    //! Attempts to create the map and detail images.
    void _create_images();

public:
    //! Constructor.
    MapGenerator();

    //! Generates the map.
    void generate();

    //! Sets the seed value.
    //! \param value
    inline void set_seed(int value) { params.seed = value; }

    //! Returns the seed value.
    //! \return int
    inline int get_seed() const { return params.seed; }

    //! Sets the map size.
    //! \param size
    inline void set_map_size(int size) { params.map_size = size; }

    //! Returns the map size.
    //! \return int
    inline int get_map_size() const { return params.map_size; }

    //! Sets the map image.
    //! \param image
    inline void set_map_image(const godot::Ref<godot::Image>& image) { map_image = image; }

    //! Returns the map image.
    //! \return godot::Ref<godot::Image>
    inline const godot::Ref<godot::Image>& get_map_image() const { return map_image; }

    //! Sets the map texture.
    //! \param texture
    inline void set_map_texture(const godot::Ref<godot::ImageTexture>& texture) { map_texture = texture; }

    //! Returns the map texture.
    //! \return godot::Ref<godot::ImageTexture>
    inline const godot::Ref<godot::ImageTexture>& get_map_texture() const { return map_texture; }

    //! Sets the detail map image.
    //! \param image
    inline void set_detail_map_image(const godot::Ref<godot::Image>& image) { detail_map_image = image; }

    //! Returns the detail map image.
    //! \return godot::Ref<godot::Image>
    inline const godot::Ref<godot::Image>& get_detail_map_image() const { return detail_map_image; }

    //! Sets the detail map texture.
    //! \param texture
    inline void set_detail_map_texture(const godot::Ref<godot::ImageTexture>& texture) { detail_map_texture = texture; }

    //! Returns the detail map texture.
    //! \return godot::Ref<godot::ImageTexture>
    inline const godot::Ref<godot::ImageTexture>& get_detail_map_texture() const { return detail_map_texture; }

    // Sets the max number of floors to generate.
    inline void set_max_floors_placed(int num) { params.max_floors = num; }

    // Returns the max number of floors to generate.
    inline int get_max_floors_placed() { return params.max_floors; }

    //! Registers the methods for this class.
    static void _register_methods();
};

