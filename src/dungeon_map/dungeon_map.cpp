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
#include <dungeon_map/dungeon_map.hpp>
#include <dungeon_map/dungeon_map_mesh_builder.hpp>
#include <VisualServer.hpp>
#include <SurfaceTool.hpp>
#include <ArrayMesh.hpp>
#include <Shape.hpp>

using namespace godot;

DungeonMap::DungeonMap()
{
}

DungeonMap::~DungeonMap()
{

}

void DungeonMap::_notification(int p_what)
{
}

void DungeonMap::enter_world(godot::World &w)
{

}

void DungeonMap::exit_world()
{
    clear();
}

void DungeonMap::_process()
{
//    Godot::print("process dungeon map");
}

void DungeonMap::build()
{
    if (!map_image.is_valid()) {
        return;
    }

    if (!dirty) {
        return;
    }

    if (!surfaceTool.is_valid()) {
        Ref<SurfaceTool> tool = memnew(SurfaceTool);
        surfaceTool = tool;
    }

    // Clear the previous data.
    clear();

    _build_regions();
    _update_tile_neighbors();

    if (surfaceTool.is_valid()) {
        _build_region_meshes();
        _build_region_mesh_edges();
    }

    // Update all regions as not dirty.
    for (auto regionIt : regions) {
        auto region = regionIt.second;
        region->dirty = false;
    }

    dirty = false;
}

void DungeonMap::clear()
{
    valid_tiles.clear();

    for (auto tile : tiles) {
        delete tile.second;
    }
    tiles.clear();

    for (auto regionIt : regions) {
        auto region = regionIt.second;

        // Free meshes.
        if (region->mesh.is_valid()) {
            region->mesh.unref();
        }
        if (region->edge_mesh.is_valid()) {
            region->edge_mesh.unref();
        }

        // Free instance.
        if (region->mesh_instance) {
            region->mesh_instance->queue_free();
        }

        if (region->edge_mesh_instance) {
            region->edge_mesh_instance->queue_free();
        }

        // Delete collision.
        if (region->collision_shape) {
            region->collision_shape->queue_free();
        }
        if (region->collision_body) {
            region->collision_body->queue_free();
        }

        if (region->edge_collision_shape) {
            region->edge_collision_shape->queue_free();
        }
        if (region->edge_collision_body) {
            region->edge_collision_body->queue_free();
        }


        // Remove all region tiles.
        for (auto tile : region->tiles) {
            delete tile;
        }
        region->tiles.clear();

        delete region;
    }
    regions.clear();

    dirty = true;
}

void DungeonMap::_build_regions()
{
    int region_width = tiles_per_region * (int)tile_size;

    // Build out our region and the cells.
    for (int x = 0; x < region_size; x++) {
        for (int y = 0; y < region_size; y++) {
            Vector2 id = Vector2(x, y);
            if (regions.find(godot::String(id).alloc_c_string()) != regions.end())
                continue;

            auto region = new Region();
            region->dirty = true;
            region->aabb = AABB(
                    Vector3(
                            x * region_width,
                            0.0f,
                            y * region_width * -1.0f
                    ),
                    Vector3(
                            region_width,
                            2,
                            region_width
                    )
            );
            region->id = Vector2(region->aabb.position.x, region->aabb.position.z);

            region->transform.translated(region->aabb.position);
            regions[get_id(region->id)] = region;
            _build_tiles(region->id);
        }
    }
}

void DungeonMap::_build_tiles(const godot::Vector2& region_id)
{
    // Build out the tiles per region.
    for (int x = 0; x < tiles_per_region; x++) {
        for (int y = 0; y < tiles_per_region; y++) {
            if (regions.find(get_id(region_id)) == regions.end()) {
                continue;
            }

            // Create our tile.
            Region* region = regions[get_id(region_id)];
            Tile* tile = new Tile();
            tile->local_position = Vector2(x, y);
            tile->region_id = region_id;
            tile->aabb = AABB(
                    Vector3(
                            (x * tile_size) + region->aabb.position.x,
                            0.0f,
                            (y * tile_size * -1.0f) + region->aabb.position.z
                    ),
                    Vector3(
                            tile_size,
                            tile->height,
                            tile_size
                    )
            );
            tile->id = Vector2(tile->aabb.position.x, tile->aabb.position.z);

            int px = abs((int)tile->id.x);
            int py = abs((int)tile->id.y * -1);

//            Godot::print("Finding location: " + godot::String(std::to_string(px).c_str()) + ", " + godot::String(std::to_string(py).c_str()) + " for " + godot::String(tile->id));
            map_image->lock();
            Color pixel = map_image->get_pixel(px, py);
            map_image->unlock();

            if (pixel != Color(0, 0, 0)) {
                tile->type = FLOOR;
            } else {
                tile->height = -1;
            }

            // Save our tile.
            tile->transform.translated(tile->aabb.position);
            tiles[get_id(tile->id)] = tile;

            // Add tile to the region. Check for empty so
            // we know how to build the mesh.
            if (tile->type != EMPTY) {
                region->tiles.push_back(tile);
            }
        }
    }
}

void DungeonMap::_update_tile_neighbors()
{
    // Update the neighbors.
    for (auto tileIt : tiles) {
        auto tile = tileIt.second;

        tile->is_edge = false;
        if (tile->neighbors.size() == 0) {
            tile->neighbors.resize((uint8_t)MAX_TILE_NEIGHBORS);
        }

        Vector2 tile_id = tile->id;
        Vector2 north_pos = Vector2(tile_id.x, tile_id.y - tile_size);
        Vector2 east_pos = Vector2(tile_id.x + tile_size, tile_id.y);
        Vector2 south_pos = Vector2(tile_id.x, tile_id.y + tile_size);
        Vector2 west_pos = Vector2(tile_id.x - tile_size, tile_id.y);

        Vector2 north_east_pos = Vector2(tile_id.x + tile_size, tile_id.y - tile_size);
        Vector2 north_west_pos = Vector2(tile_id.x - tile_size, tile_id.y - tile_size);
        Vector2 south_east_pos = Vector2(tile_id.x + tile_size, tile_id.y + tile_size);
        Vector2 south_west_pos = Vector2(tile_id.x - tile_size, tile_id.y + tile_size);

        // Set the north tile.
        Tile* nTile = find_tile(north_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH] = nTile;
            tile->has_north_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH] = NULL;
            tile->is_edge = true;
        }

        // Set the east cell.
        nTile = find_tile(east_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_EAST] = nTile;
            tile->has_east_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_EAST] = NULL;
            tile->is_edge = true;
        }

        // Set the south cell.
        nTile = find_tile(south_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH] = nTile;
            tile->has_south_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH] = NULL;
            tile->is_edge = true;
        }

        // Set the west cell.
        nTile = find_tile(west_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_WEST] = nTile;
            tile->has_west_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_WEST] = NULL;
            tile->is_edge = true;
        }

        // Set the north-east cell.
        nTile = find_tile(north_east_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH_EAST] = nTile;
            tile->has_north_east_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH_EAST] = NULL;
            tile->is_edge = true;
        }

        // Set the north-west cell.
        nTile = find_tile(north_west_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH_WEST] = nTile;
            tile->has_north_west_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_NORTH_WEST] = NULL;
            tile->is_edge = true;
        }

        // Set the south-east cell.
        nTile = find_tile(south_east_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH_EAST] = nTile;
            tile->has_south_east_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH_EAST] = NULL;
            tile->is_edge = true;
        }

        // Set the south-west cell.
        nTile = find_tile(south_west_pos);
        if (nTile) {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH_WEST] = nTile;
            tile->has_south_west_tile = true;
        } else {
            tile->neighbors[(uint8_t)Neighbor::NEIGHBOR_SOUTH_WEST] = NULL;
            tile->is_edge = true;
        }

        // Tiles with no edges are considered valid tiles.
        // Perhaps consider having non-edge valid tiles and
        // included-edge valid tiles.
        //
        // This prevents users from spawning on edge tiles.
        if (tile->type != EMPTY && !tile->is_edge) {
            valid_tiles.push_back(tile);
        }
    }
}

void DungeonMap::_build_region_meshes()
{
    for (auto regionIt : regions) {
        auto region = regionIt.second;

        if (region->tiles.size() == 0) continue;

        surfaceTool->clear();
        surfaceTool->begin(Mesh::PRIMITIVE_TRIANGLES);
        surfaceTool->add_to_format(Mesh::ARRAY_FORMAT_NORMAL);
        for (auto tile : region->tiles) {
            DungeonMapMeshBuilder::add_mesh_tile(this, surfaceTool.ptr(), tile->id, false, false);
//            DungeonMapMeshBuilder::add_mesh_tile(this, surfaceTool.ptr(), tile->id, ceiling_height, true, false);
        }

//        surfaceTool->generate_normals(); // BROKEN...
//        surfaceTool->generate_tangents();
        surfaceTool->index();
        region->mesh = surfaceTool->commit();

        surfaceTool->generate_normals();
        surfaceTool->commit(region->mesh);

        // Create the mesh instance.
        region->mesh_instance = memnew(MeshInstance);
        region->mesh_instance->set_flag(GeometryInstance::FLAG_USE_BAKED_LIGHT, true);
        region->mesh_instance->set_mesh(region->mesh);

        owner->add_child(region->mesh_instance);
        region->mesh_instance->set_owner(owner);
//        region->instance = VisualServer::instance_create2(region->mesh->get_rid(), owner->get_world()->get_scenario());
//        VisualServer::instance_set_transform(region->instance, region->transform);
//        VisualServer::instance_set_visible(region->instance, true);

        // Create the collision shape.
        _create_region_collision(region->id);
    }
}

void DungeonMap::_build_region_mesh_edges()
{
    for (auto regionIt : regions) {
        auto region = regionIt.second;
        if (region->tiles.size() == 0) continue;

        surfaceTool->clear();
        surfaceTool->begin(Mesh::PRIMITIVE_TRIANGLES);
        surfaceTool->add_to_format(Mesh::ARRAY_FORMAT_NORMAL);
        for (auto tile : region->tiles) {
            DungeonMapMeshBuilder::add_mesh_tile_edge(this, surfaceTool.ptr(), tile->id, ceiling_height, true, false);
        }

//        surfaceTool->generate_normals(); // BROKEN...
//        surfaceTool->generate_tangents();
        surfaceTool->index();
        region->edge_mesh = surfaceTool->commit();

        // Create the edge mesh instance.
        region->edge_mesh_instance = memnew(MeshInstance);
        region->edge_mesh_instance->set_flag(GeometryInstance::FLAG_USE_BAKED_LIGHT, true);
        region->edge_mesh_instance->set_mesh(region->edge_mesh);

        owner->add_child(region->edge_mesh_instance);
        region->edge_mesh_instance->set_owner(owner);
//        region->edge_instance = VisualServer::instance_create2(region->edge_mesh->get_rid(), owner->get_world()->get_scenario());
//        VisualServer::instance_set_transform(region->edge_instance, region->transform);
//        VisualServer::instance_set_visible(region->edge_instance, true);

        // Create the collision shape.
        _create_region_edge_collision(region->id);
    }
}

void DungeonMap::_create_collision(godot::StaticBody*& collision_body, godot::CollisionShape*& collision_shape, godot::Ref<godot::Mesh> mesh, const godot::Transform& transform)
{
    if (mesh.is_null())
        return;

    Ref<Shape> shape = mesh->create_trimesh_shape();
    if (shape.is_null())
        return;

    collision_body = memnew(StaticBody);
    collision_shape = memnew(CollisionShape);
    collision_shape->set_shape(shape);
    collision_body->add_child(collision_shape);
    collision_shape->set_owner(collision_body);

    // Build the collision.
    // String("region_") + region->id + "_col"
    collision_body->set_name("collision_body");
    collision_body->set_transform(transform);

    owner->add_child(collision_body);
    collision_body->set_owner(owner);
}

void DungeonMap::_create_region_collision(const godot::Vector2& region_id)
{
    Region* region = find_region(region_id);
    if (!region || !region->dirty) return;

    if (region->mesh.is_null())
        return;

    if (region->collision_body != NULL) {
        region->collision_body->remove_child(region->collision_shape);
        owner->remove_child(region->collision_body);
        region->collision_body->set_owner(NULL);

        memdelete(region->collision_shape);
        memdelete(region->collision_body);
    }

    _create_collision(region->collision_body, region->collision_shape, region->mesh, region->transform);
}

void DungeonMap::_create_region_edge_collision(const godot::Vector2& region_id)
{
    Region* region = find_region(region_id);
    if (!region || !region->dirty) return;

    if (region->edge_mesh.is_null())
        return;

    if (region->edge_collision_body != NULL) {
        region->edge_collision_body->remove_child(region->edge_collision_shape);
        owner->remove_child(region->edge_collision_body);
        region->edge_collision_body->set_owner(NULL);

        memdelete(region->edge_collision_shape);
        memdelete(region->edge_collision_body);
    }

    _create_collision(region->edge_collision_body, region->edge_collision_shape, region->edge_mesh, region->transform);
}

void DungeonMap::_update_visibility()
{

}

void DungeonMap::_update_transform()
{

}

void DungeonMap::_update_material()
{

}

Region* DungeonMap::find_region(const godot::Vector2& region_id)
{
    auto entry = regions.find(get_id(region_id));
    if (entry == regions.end()) {
        return NULL;
    }
    return entry->second;
}

Tile* DungeonMap::find_tile(const godot::Vector2& tile_id)
{
    auto entry = tiles.find(get_id(tile_id));
    if (entry == tiles.end()) {
        return NULL;
    }
    return entry->second;
}

godot::Vector2 DungeonMap::get_random_map_location()
{
    if (valid_tiles.size() == 0) {
        Godot::print("No valid tiles to spawn on!!!");
        return Vector2();
    }

    Vector2 position = valid_tiles[rand() % valid_tiles.size()]->id;
    Tile* tile = find_tile(position);
//    float floor_size_half = (float)(get_dungeon_params().floor_size) / 2.0f;
    if (!tile) {
        return Vector2();
    }

    return Vector2(
            position.x, // + floor_size_half,
            position.y // - floor_size_half
    );
}

godot::Vector2 DungeonMap::get_tile_position(const godot::Vector2& position)
{
    return godot::Vector2(
            floor(position.x - ((int)position.x % (int)tile_size)),
            floor(position.y - ((int)position.y % (int)tile_size))
    );
}

bool DungeonMap::is_valid_position(const godot::Vector2& position)
{
    Tile* tile = find_tile(get_tile_position(position));
    if (!tile) return false;

    return tile->type != EMPTY;
}

void DungeonMap::set_map_tile_color(const godot::Vector2& tile_id, godot::Color color)
{

}

godot::Array DungeonMap::get_meshes()
{
    Array meshArr;
    for (auto region : regions) {
        meshArr.push_back(region.second->mesh);
    }
    return meshArr;
}

godot::Array DungeonMap::get_edge_meshes()
{
    Array meshArr;
    for (auto region : regions) {
        meshArr.push_back(region.second->edge_mesh);
    }
    return meshArr;
}

void DungeonMap::_register_methods()
{
    godot::register_method("_process", &DungeonMap::_process);

    godot::register_method("build", &DungeonMap::build);
    godot::register_method("clear", &DungeonMap::clear);

    godot::register_method("set_map_image", &DungeonMap::set_map_image);
    godot::register_method("set_surface_tool", &DungeonMap::set_surface_tool);

    godot::register_method("get_random_map_location", &DungeonMap::get_random_map_location);

    godot::register_method("set_region_size", &DungeonMap::set_region_size);
    godot::register_method("get_region_size", &DungeonMap::get_region_size);

    godot::register_method("set_tiles_per_region", &DungeonMap::set_tiles_per_region);
    godot::register_method("get_tiles_per_region", &DungeonMap::get_tiles_per_region);

    godot::register_method("set_tile_size", &DungeonMap::set_tile_size);
    godot::register_method("get_tile_size", &DungeonMap::get_tile_size);

    godot::register_method("set_ceiling_height", &DungeonMap::set_ceiling_height);
    godot::register_method("get_ceiling_height", &DungeonMap::get_ceiling_height);


    godot::register_method("get_meshes", &DungeonMap::get_meshes);
    godot::register_method("get_edge_meshes", &DungeonMap::get_edge_meshes);
}
