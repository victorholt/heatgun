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

#include <core/Godot.hpp>
#include <Reference.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>

class MapGenerator : public godot::GodotScript<godot::Reference>
{
    GODOT_CLASS(MapGenerator)

private:
    /// Seed for generating our map.
    int seed = 0;

    /// Reference to the map image.
    godot::Ref<godot::Image> map_img;

    /// Reference to the map texture.
    godot::Ref<godot::ImageTexture> map_texture;

    /// Reference to the detail map image.
    godot::Ref<godot::Image> detail_map_img;

    /// Reference to the detail map texture.
    godot::Ref<godot::Image> detail_map_texture;

public:
    //! Constructor.
    MapGenerator();

    //! Sets the seed value.
    //! \param value
    inline void set_seed(int value) { seed = value; }

    //! Returns the seed value.
    //! \return int
    inline int get_seed() const { return seed; }

    //! Sets the map image.
    //! \param image
    inline void set_map_image(const godot::Ref<godot::Image>& image) { map_img = image; }

    //! Returns the map image.
    //! \return godot::Ref<godot::Image>
    inline const godot::Ref<godot::Image>& get_map_image() const { return map_img; }

    //! Sets the map texture.
    //! \param texture
    inline void set_map_texture(const godot::Ref<godot::ImageTexture>& texture) { map_texture = texture; }

    //! Returns the map texture.
    //! \return godot::Ref<godot::ImageTexture>
    inline const godot::Ref<godot::ImageTexture>& get_map_texture() const { return map_texture; }

    //! Sets the detail map image.
    //! \param image
    inline void set_detail_map_image(const godot::Ref<godot::Image>& image) { detail_map_img = image; }

    //! Returns the detail map image.
    //! \return godot::Ref<godot::Image>
    inline const godot::Ref<godot::Image>& get_detail_map_image() const { return detail_map_img; }

    //! Sets the detail map texture.
    //! \param texture
    inline void set_detail_map_texture(const godot::Ref<godot::ImageTexture>& texture) { detail_map_texture = texture; }

    //! Returns the detail map texture.
    //! \return godot::Ref<godot::ImageTexture>
    inline const godot::Ref<godot::ImageTexture>& get_detail_map_texture() const { return detail_map_texture; }

    //! Registers the methods for this class.
    static void _register_methods();
};

