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
#include <SurfaceTool.hpp>

class DungeonMap;

class DungeonMapMeshBuilder
{
public:
    // Calculating the uv for the tile meshes.
    enum UVType
    {
        UV_XY = 0,
        UV_XZ,
        UV_YZ,
        UV_ZY
    };

    // Creates a tile mesh based on the given tile id.
    static void add_mesh_tile(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const godot::Vector2& tile_id, int height, bool inverse, bool calcNormals);
    // Creates a tile mesh based on the given tile id.
    static void add_mesh_tile(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const godot::Vector2& tile_id, bool inverse, bool calcNormals);

    // Calculate the normals for a vertex.
    static void calc_normals(
            const godot::Vector3& v0,
            const godot::Vector3& v1,
            const godot::Vector3& v2,
            godot::Vector3& ov0,
            godot::Vector3& ov1,
            godot::Vector3& ov2,
            bool flipY = true);

    // Creates a tile mesh based on the given tile id.
    static void add_mesh_tile_edge(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const godot::Vector2& tile_id, int height, bool inverse, bool calcNormals);
    // Creates a tile mesh based on the given tile id.
    static void add_mesh_tile_edge(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const godot::Vector2& tile_id, bool inverse, bool calcNormals);

    // Creates a mesh from a given aabb.
    static godot::RID create_mesh_from_aabb(const godot::AABB& aabb);
    // Creates mesh lines from a given aabb.
    static godot::RID create_mesh_lines_from_aabb(const godot::AABB& aabb);

    // Returns a uv coordinate for a vertex.
    static godot::Vector2 get_uv(UVType uvType, const godot::Vector3& vertex, float scale = 1.0f);
};
