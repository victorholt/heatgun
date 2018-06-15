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

void MapGenerator::_register_methods()
{
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

//    godot::register_property((char*)"base/seed", &MapGenerator::seed, godot::String("0"));
}