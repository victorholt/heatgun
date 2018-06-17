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
#include <map_generator/map_generator.hpp>

MapGenerator::MapGenerator()
{

}

void MapGenerator::generate()
{
    srand((unsigned)params.seed);

    // Reset our params.
    params.floors_placed = 0;
    params.lower_bound = godot::Vector2(0, 0);
    params.upper_bound = godot::Vector2(0, 0);

    // Clear the image.
    if (!map_image.is_valid()) {
        _create_images();
    } else {
        _create_images();
    }

    _build_floors();

    // Update our map texture.
    map_texture->set_data(map_image);
}

void MapGenerator::_build_floors()
{
    if (params.is_generating) {
        return;
    }
    params.is_generating = true;

    // Set the initial position.
    params.rand_x = rand() % params.map_size - params.floor_size;
    params.rand_y = rand() % params.map_size - params.floor_size;

    params.current_x = params.rand_x - (params.rand_x % params.floor_size);
    params.current_y = params.rand_y - (params.rand_y % params.floor_size);
    params.lower_bound.x = params.current_x;
    params.upper_bound.y = params.current_y;

    // Update our bounds so that the initial start is what
    // we'd expect it to be.
    _update_bounds();

    // Create our floors for the dungeon.
    while (params.floors_placed < params.max_floors) {
        _place_floor();
    }

    // Update the status to let us know that we're finished.
    params.is_generating = false;
}

void MapGenerator::_place_floor()
{
    // Add our floor tile to the image.
    map_image->lock();
    for (int x = 0; x < params.floor_size; x++) {
        for (int y = 0; y < params.floor_size; y++) {
            map_image->set_pixel(params.current_x + x, params.current_y + y, godot::Color(1, 1, 1));
        }
    }
    map_image->unlock();

    // Increment the number of floors.
    params.floors_placed += 1;

    // Determine the next location for the floor tile.
    params.dir = rand() % 6;
    switch (params.dir) {
        case 1:
            params.current_x += params.floor_size;
            break;
        case 2:
            params.current_y += params.floor_size;
            break;
        case 3:
            params.current_x -= params.floor_size;
            break;
        case 4:
            params.current_y -= params.floor_size;
            break;
    }

    _update_bounds();
}

void MapGenerator::_update_bounds()
{
    // Check if we are outside of the room
    if (params.current_x < params.floor_size)
        params.current_x = params.floor_size;
    if (params.current_x > int(params.map_size) - (params.floor_size * 2))
        params.current_x = int(params.map_size) - (params.floor_size * 2);

    if (params.current_y < params.floor_size)
        params.current_y = params.floor_size;
    if (params.current_y > int(params.map_size) - (params.floor_size * 2))
        params.current_y = int(params.map_size) - (params.floor_size * 2);

    // Store the top-left and bottom-rigth corners of the map.
    if (params.current_x < params.lower_bound.x)
        params.lower_bound.x = params.current_x;
    if (params.current_x > params.upper_bound.x)
        params.upper_bound.x = params.current_x;
    if (params.current_y < params.lower_bound.y)
        params.lower_bound.y = params.current_y;
    if (params.current_y > params.upper_bound.y)
        params.upper_bound.y = params.current_y;
}

void MapGenerator::_create_images()
{
    if (map_image.is_valid()) {
        map_image.unref();
    }
    if (map_texture.is_valid()) {
        map_texture.unref();
    }

    godot::Ref<godot::Image> img = memnew(godot::Image);
    img->create(
            params.map_size,
            params.map_size,
            false,
            godot::Image::FORMAT_RGB8
    );

    map_image = img;

    godot::Ref<godot::ImageTexture> tex = memnew(godot::ImageTexture);
    tex->create_from_image(map_image);
    map_texture = tex;
}

void MapGenerator::_register_methods()
{
    godot::register_method("generate", &MapGenerator::generate);

    godot::register_method("set_seed", &MapGenerator::set_seed);
    godot::register_method("get_seed", &MapGenerator::get_seed);

    godot::register_method("set_map_image", &MapGenerator::set_map_image);
    godot::register_method("get_map_image", &MapGenerator::get_map_image);
    godot::register_method("set_map_texture", &MapGenerator::set_map_texture);
    godot::register_method("get_map_texture", &MapGenerator::get_map_texture);

    godot::register_method("set_detail_map_image", &MapGenerator::set_detail_map_image);
    godot::register_method("get_detail_map_image", &MapGenerator::get_detail_map_image);
    godot::register_method("set_detail_map_texture", &MapGenerator::set_detail_map_texture);
    godot::register_method("get_detail_map_texture", &MapGenerator::get_detail_map_texture);

    godot::register_property(
            "seed",
            &MapGenerator::set_seed,
            &MapGenerator::get_seed,
            0,
            GODOT_METHOD_RPC_MODE_DISABLED,
            GODOT_PROPERTY_USAGE_EDITOR,
            GODOT_PROPERTY_HINT_NONE
    );

    godot::register_property(
            "map_size",
            &MapGenerator::set_map_size,
            &MapGenerator::get_map_size,
            0,
            GODOT_METHOD_RPC_MODE_DISABLED,
            GODOT_PROPERTY_USAGE_EDITOR,
            GODOT_PROPERTY_HINT_NONE
    );

    godot::register_property(
            "max_floors",
            &MapGenerator::set_max_floors_placed,
            &MapGenerator::get_max_floors_placed,
            0,
            GODOT_METHOD_RPC_MODE_DISABLED,
            GODOT_PROPERTY_USAGE_EDITOR,
            GODOT_PROPERTY_HINT_NONE
    );

//    godot::register_property(
//            "map_texture",
//            &MapGenerator::set_map_texture,
//            &MapGenerator::get_seed,
//            0,
//            GODOT_METHOD_RPC_MODE_DISABLED,
//            GODOT_PROPERTY_USAGE_EDITOR,
//            GODOT_PROPERTY_HINT_NONE
//    );

//    godot::register_property(
//            "detail_texture",
//            &MapGenerator::set_seed,
//            &MapGenerator::get_seed,
//            0,
//            GODOT_METHOD_RPC_MODE_DISABLED,
//            GODOT_PROPERTY_USAGE_EDITOR,
//            GODOT_PROPERTY_HINT_NONE
//    );
}
