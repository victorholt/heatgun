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
#include <dungeon_map/dungeon_map_mesh_builder.hpp>
#include <dungeon_map/dungeon_map.hpp>
#include <VisualServer.hpp>
#include <ArrayMesh.hpp>

using namespace godot;

// Using OpenGL (right-handed coord system) convention where -Z is forward...

void DungeonMapMeshBuilder::calc_normals(
        const godot::Vector3& v0,
        const godot::Vector3& v1,
        const godot::Vector3& v2,
        godot::Vector3& ov0,
        godot::Vector3& ov1,
        godot::Vector3& ov2,
        bool flipY)
{
    Vector3 ev0 = v0;
    Vector3 ev1 = v1;
    Vector3 ev2 = v2;

    Vector3 edge1 = ev1 - ev0;
    Vector3 edge2 = ev2 - ev0;

    ov0 = ov1 = ov2 = edge1.cross(edge2).normalized();

    if (flipY) {
        ov0.y *= -1.0f;
        ov1.y *= -1.0f;
        ov2.y *= -1.0f;
    }
}

void DungeonMapMeshBuilder::add_mesh_tile(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const Vector2& tile_id, int height, bool inverse, bool calcNormals)
{
    Tile* tile = dungeon_map->find_tile(tile_id);
    if (!tile) {
        return;
    }
    float scale = dungeon_map->get_tile_size();
    Vector2 origin = Vector2(tile->aabb.position.x, tile->aabb.position.z);
    Vector3 position = tile->aabb.position;

    if (height == -99999) {
        height = tile->height;
    }

    // Vertices for our mesh.
    Vector3 v00;
    Vector3 v01;
    Vector3 v02;

    Vector3 v10;
    Vector3 v11;
    Vector3 v12;

    // Calculated normals for our mesh.
    Vector3 nv00;
    Vector3 nv01;
    Vector3 nv02;

    Vector3 nv10;
    Vector3 nv11;
    Vector3 nv12;

    if (!inverse) {
        v00 = Vector3(origin.x, height, origin.y);
        v01 = Vector3(origin.x, height, origin.y - scale);
        v02 = Vector3(origin.x + scale, height, origin.y);

        v10 = Vector3(origin.x + scale, height, origin.y);
        v11 = Vector3(origin.x, height, origin.y - scale);
        v12 = Vector3(origin.x + scale, height, origin.y - scale);

        if (calcNormals) {
            calc_normals(
                    v00, v01, v02,
                    nv00, nv01, nv02
            );

            calc_normals(
                    v10, v11, v12,
                    nv10, nv11, nv12
            );
        }

        // Triangle 1
        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y), scale));
        tool->add_vertex(v00);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y + scale), scale));
        tool->add_vertex(v01);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y), scale));
        tool->add_vertex(v02);
        tool->add_normal(nv02);

        // Triangle 2
        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y), scale));
        tool->add_vertex(v10);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y + scale), scale));
        tool->add_vertex(v11);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y + scale), scale));
        tool->add_vertex(v12);
        tool->add_normal(nv12);
    } else {
        v00 = Vector3(origin.x, height, origin.y);
        v01 = Vector3(origin.x + scale, height, origin.y);
        v02 = Vector3(origin.x, height, origin.y - scale);

        v10 = Vector3(origin.x + scale, height, origin.y);
        v11 = Vector3(origin.x + scale, height, origin.y - scale);
        v12 = Vector3(origin.x, height, origin.y - scale);

        if (calcNormals) {
            calc_normals(
                    v00, v01, v02,
                    nv02, nv01, nv00
            );

            calc_normals(
                    v10, v11, v12,
                    nv10, nv11, nv12
            );
        }

        // Triangle 1
        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y), scale));
        tool->add_vertex(v00);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y), scale));
        tool->add_vertex(v01);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y + scale), scale));
        tool->add_vertex(v02);
        tool->add_normal(nv02);

        // Triangle 2
        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y), scale));
        tool->add_vertex(v10);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x + scale, height, position.y + scale), scale));
        tool->add_vertex(v11);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_XZ, Vector3(position.x, height, position.y + scale), scale));
        tool->add_vertex(v12);
        tool->add_normal(nv12);
    }
}

void DungeonMapMeshBuilder::add_mesh_tile(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const Vector2& tile_id, bool inverse, bool calcNormals)
{
    add_mesh_tile(dungeon_map, tool, tile_id, -99999, inverse, calcNormals);
}

void DungeonMapMeshBuilder::add_mesh_tile_edge(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const Vector2& tile_id, int height, bool inverse, bool calcNormals)
{
    Tile* tile = dungeon_map->find_tile(tile_id);
    if (!tile || tile->type == TileType::EMPTY) {
        return;
    }

    Vector2 origin = Vector2(tile->aabb.position.x, tile->aabb.position.z);
    Vector3 position = tile->aabb.position;
    if (height == -99999) {
        height = tile->height;
    }
    float length = dungeon_map->get_tile_size();
    float edge_slant = 0.0f;

    // Calculated normals for our mesh.
    Vector3 nv00;
    Vector3 nv01;
    Vector3 nv02;

    Vector3 nv10;
    Vector3 nv11;
    Vector3 nv12;

    if (!tile->has_north_tile || tile->GetNeightbor(NEIGHBOR_NORTH)->type == EMPTY) {
        Vector3 v01 = Vector3(origin.x - edge_slant, 0.0f, origin.y - length - edge_slant);
        Vector3 v02 = Vector3(origin.x + length, height, origin.y - length);
        Vector3 v03 = Vector3(origin.x, height, origin.y - length);

        Vector3 v11 = Vector3(origin.x - edge_slant, 0.0f, origin.y - length - edge_slant);
        Vector3 v12 = Vector3(origin.x + length + edge_slant, 0.0f, origin.y - length - edge_slant);
        Vector3 v13 = Vector3(origin.x + length, height, origin.y - length);

        if (calcNormals) {
            calc_normals(
                    v01, v02, v03,
                    nv00, nv01, nv02
            );

            calc_normals(
                    v11, v12, v13,
                    nv10, nv11, nv12
            );
        }

        // Check if we need to inverse the mesh.
        if (inverse) {
            Vector3 tmp = v02;
            v02 = v03;
            v03 = tmp;

            tmp = v12;
            v12 = v13;
            v13 = tmp;
        }

        // Triangle #1
        tool->add_uv(get_uv(UVType::UV_XY, v01));
        tool->add_vertex(v01);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_XY, v02));
        tool->add_vertex(v02);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_XY, v03));
        tool->add_vertex(v03);
        tool->add_normal(nv02);

        // Triangle #2
        tool->add_uv(get_uv(UVType::UV_XY, v11));
        tool->add_vertex(v11);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_XY, v12));
        tool->add_vertex(v12);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_XY, v13));
        tool->add_vertex(v13);
        tool->add_normal(nv12);
    }

    if (!tile->has_east_tile || tile->GetNeightbor(NEIGHBOR_EAST)->type == EMPTY) {
        Vector3 v01 = Vector3(origin.x + length + edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v02 = Vector3(origin.x + length, height, origin.y);
        Vector3 v03 = Vector3(origin.x + length, height, origin.y - length);

        Vector3 v11 = Vector3(origin.x + length + edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v12 = Vector3(origin.x + length, height, origin.y - length);
        Vector3 v13 = Vector3(origin.x + length + edge_slant, 0.0f, origin.y - length - edge_slant);

        // Check if we need to inverse the mesh.
        if (inverse) {
            Vector3 tmp = v02;
            v02 = v03;
            v03 = tmp;

            tmp = v12;
            v12 = v13;
            v13 = tmp;
        }

        if (calcNormals) {
//            calc_normals(
//                    v01, v02, v03,
//                    nv00, nv01, nv02
//            );
//
//            calc_normals(
//                    v11, v12, v13,
//                    nv10, nv11, nv12
//            );
        }

        // Triangle #1
        tool->add_uv(get_uv(UVType::UV_ZY, v01));
        tool->add_vertex(v01);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_ZY, v02));
        tool->add_vertex(v02);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_ZY, v03));
        tool->add_vertex(v03);
        tool->add_normal(nv02);

        // Triangle #2
        tool->add_uv(get_uv(UVType::UV_ZY, v11));
        tool->add_vertex(v11);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_ZY, v12));
        tool->add_vertex(v12);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_ZY, v13));
        tool->add_vertex(v13);
        tool->add_normal(nv12);
    }

    if (!tile->has_south_tile || tile->GetNeightbor(NEIGHBOR_SOUTH)->type == EMPTY) {
        Vector3 v01 = Vector3(origin.x - edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v02 = Vector3(origin.x, height, origin.y);
        Vector3 v03 = Vector3(origin.x + length, height, origin.y);

        Vector3 v11 = Vector3(origin.x - edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v12 = Vector3(origin.x + length, height, origin.y);
        Vector3 v13 = Vector3(origin.x + length + edge_slant, 0.0f, origin.y + edge_slant);

        // Check if we need to inverse the mesh.
        if (inverse) {
            Vector3 tmp = v02;
            v02 = v03;
            v03 = tmp;

            tmp = v12;
            v12 = v13;
            v13 = tmp;
        }

        if (calcNormals) {
//            calc_normals(
//                    v01, v02, v03,
//                    nv00, nv01, nv02
//            );
//
//            calc_normals(
//                    v11, v12, v13,
//                    nv10, nv11, nv12
//            );
        }

        // Triangle #1
        tool->add_uv(get_uv(UVType::UV_XY, v01));
        tool->add_vertex(v01);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_XY, v02));
        tool->add_vertex(v02);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_XY, v03));
        tool->add_vertex(v03);
        tool->add_normal(nv02);

        // Triangle #2
        tool->add_uv(get_uv(UVType::UV_XY, v11));
        tool->add_vertex(v11);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_XY, v12));
        tool->add_vertex(v12);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_XY, v13));
        tool->add_vertex(v13);
        tool->add_normal(nv12);
    }

    if (!tile->has_west_tile || tile->GetNeightbor(NEIGHBOR_WEST)->type == EMPTY) {
        Vector3 v01 = Vector3(origin.x - edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v02 = Vector3(origin.x, height, origin.y - length);
        Vector3 v03 = Vector3(origin.x, height, origin.y);

        Vector3 v11 = Vector3(origin.x - edge_slant, 0.0f, origin.y + edge_slant);
        Vector3 v12 = Vector3(origin.x - edge_slant, 0.0f, origin.y - length - edge_slant);
        Vector3 v13 = Vector3(origin.x, height, origin.y - length);

        // Check if we need to inverse the mesh.
        if (inverse) {
            Vector3 tmp = v02;
            v02 = v03;
            v03 = tmp;

            tmp = v12;
            v12 = v13;
            v13 = tmp;
        }

        if (calcNormals) {
//            calc_normals(
//                    v01, v02, v03,
//                    nv00, nv01, nv02
//            );
//
//            calc_normals(
//                    v11, v12, v13,
//                    nv10, nv11, nv12
//            );
        }

        // Triangle #1
        tool->add_uv(get_uv(UVType::UV_ZY, v01));
        tool->add_vertex(v01);
        tool->add_normal(nv00);

        tool->add_uv(get_uv(UVType::UV_ZY, v02));
        tool->add_vertex(v02);
        tool->add_normal(nv01);

        tool->add_uv(get_uv(UVType::UV_ZY, v03));
        tool->add_vertex(v03);
        tool->add_normal(nv02);

        // Triangle #2
        tool->add_uv(get_uv(UVType::UV_ZY, v11));
        tool->add_vertex(v11);
        tool->add_normal(nv10);

        tool->add_uv(get_uv(UVType::UV_ZY, v12));
        tool->add_vertex(v12);
        tool->add_normal(nv11);

        tool->add_uv(get_uv(UVType::UV_ZY, v13));
        tool->add_vertex(v13);
        tool->add_normal(nv12);
    }
}

void DungeonMapMeshBuilder::add_mesh_tile_edge(DungeonMap* dungeon_map, godot::SurfaceTool* tool, const Vector2& tile_id, bool inverse, bool calcNormals)
{
    add_mesh_tile_edge(dungeon_map, tool, tile_id, -99999, inverse);
}

RID DungeonMapMeshBuilder::create_mesh_from_aabb(const AABB& aabb)
{
    // Create the debug render aabb instance.
    RID mesh = godot::VisualServer::mesh_create();
    Vector3 pos = aabb.position;
    Vector3 size = aabb.size;

    SurfaceTool st;
    st.begin(Mesh::PRIMITIVE_TRIANGLES);

    // Front
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));

    // Back
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));

    // Left
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));

    // Right
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));

    // Top
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));

    // Bottom
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));

    st.index();
    Ref<ArrayMesh> arrMesh = st.commit();

    VisualServer::mesh_add_surface_from_arrays(
            mesh,
            VisualServer::PRIMITIVE_TRIANGLES,
            arrMesh->surface_get_arrays(0)
    );

    return mesh;
}

RID DungeonMapMeshBuilder::create_mesh_lines_from_aabb(const AABB& aabb)
{
    RID mesh = VisualServer::mesh_create();
    Vector3 pos = aabb.position;
    Vector3 size = aabb.size;
    SurfaceTool st;
    st.begin(Mesh::PRIMITIVE_LINES);

    // Front
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));

    // Back
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));

    // Left
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x, pos.y, pos.z));

    // Right
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y + size.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));

    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z - size.z));
    st.add_vertex(Vector3(pos.x + size.x, pos.y, pos.z));

    st.index();
    Ref<ArrayMesh> arrMesh = st.commit();

    VisualServer::mesh_add_surface_from_arrays(
            mesh,
            VisualServer::PRIMITIVE_LINES,
            arrMesh->surface_get_arrays(0)
    );

    arrMesh.unref();
    return mesh;
}

Vector2 DungeonMapMeshBuilder::get_uv(UVType uvType, const Vector3& vertex, float scale)
{
    Vector2 ret;
    if (uvType == UVType::UV_XY) {
        ret = Vector2(vertex.x / scale, vertex.y / scale);
    } else if (uvType == UVType::UV_XZ) {
        ret = Vector2(vertex.x / scale, vertex.z / scale);
    } else if (uvType == UVType::UV_YZ) {
        ret = Vector2(vertex.y / scale, vertex.z / scale);
    } else if (uvType == UVType::UV_ZY) {
        ret = Vector2(vertex.z / scale, vertex.y / scale);
    }
    return ret;
}
