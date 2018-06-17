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
#include <Spatial.hpp>
#include <Mesh.hpp>
#include <StaticBody.hpp>
#include <CollisionShape.hpp>
#include <Image.hpp>
#include <World.hpp>
#include <Vector2.hpp>
#include <SurfaceTool.hpp>
#include <GeometryInstance.hpp>
#include <MeshInstance.hpp>

// Types of tiles that can be generated.
enum TileType
{
    EMPTY = 0,
    FLOOR,
    WALL,
    WATER,
    MAX_TILE_TYPES
};

// Tile neighbors
enum Neighbor
{
    NEIGHBOR_NORTH = 0,
    NEIGHBOR_EAST,
    NEIGHBOR_SOUTH,
    NEIGHBOR_WEST,

    NEIGHBOR_NORTH_EAST,
    NEIGHBOR_NORTH_WEST,
    NEIGHBOR_SOUTH_EAST,
    NEIGHBOR_SOUTH_WEST,

    MAX_TILE_NEIGHBORS
};

// Tile that makes up a level.
class Tile
{
GODOT_CLASS(Tile)

public:
    // Position of the tile.
    godot::Vector2 id;
    // Region id for the tile.
    godot::Vector2 region_id;
    // Local position relative to the region.
    godot::Vector2 local_position;
    // Type of tile when generating the mesh.
    TileType type = TileType::EMPTY;
    // Tile Neighbors
    std::vector<Tile*> neighbors;

    // AABB for the tile.
    godot::AABB aabb;

    // Height of the cell.
    int height = 0;

    // Flag for whether or not this is an edge tile.
    bool is_edge = false;

    // Transform for the tile.
    godot::Transform transform;

    // Has a north tile neighbor.
    bool has_north_tile = false;
    // Has a east tile neighbor.
    bool has_east_tile = false;
    // Has a south tile neighbor.
    bool has_south_tile = false;
    // Has a west tile neighbor.
    bool has_west_tile = false;
    // Has a north-east tile neighbor.
    bool has_north_east_tile = false;
    // Has a north-west tile neighbor.
    bool has_north_west_tile = false;
    // Has a south-east tile neighbor.
    bool has_south_east_tile = false;
    // Has a south-west tile neighbor.
    bool has_south_west_tile = false;

    inline Tile* GetNeightbor(Neighbor neighbor) {
        return neighbors[(int)neighbor];
    }
};

// Regions are groups of tiles.
class Region
{
public:
    // Position of the region.
    godot::Vector2 id;
    // Tiles in the region.
    std::vector<Tile*> tiles;
    // AABB for the region.
    godot::AABB aabb;

    // Region mesh instance.
//    godot::RID instance;
    // Region edge mesh instance.
//    godot::RID edge_instance;

    godot::MeshInstance* mesh_instance = nullptr;
    godot::MeshInstance* edge_mesh_instance = nullptr;

    // Reference to the land mesh.
    godot::Ref<godot::Mesh> mesh;
    // Reference to the edge mesh.
    godot::Ref<godot::Mesh> edge_mesh;

    // Reference to the collision body.
    godot::StaticBody* collision_body = NULL;
    // Reference to the collision shape.
    godot::CollisionShape* collision_shape = NULL;

    // Reference to the collision body.
    godot::StaticBody* edge_collision_body = NULL;
    // Reference to the collision shape.
    godot::CollisionShape* edge_collision_shape = NULL;

    // Transform for the region.
    godot::Transform transform;

    // Whether or not the region needs to be regenerated.
    bool dirty = true;
};

class DungeonMap : public godot::GodotScript<godot::Spatial>
{
    GODOT_CLASS(DungeonMap)

private:
    /// Regions in the map.
    std::unordered_map<std::string, Region*> regions;

    /// All tiles on the map (including empty).
    std::unordered_map<std::string, Tile*> tiles;

    /// All valid tiles on the map.
    std::vector<Tile*> valid_tiles;

    /// Reference to the map image used to build the dungeon.
    godot::Ref<godot::Image> map_image;

    /// Reference for the tool building our mesh.
    godot::Ref<godot::SurfaceTool> surfaceTool;

    /// Number of regions (region_size * region_size).
    int region_size = 1;
    /// Tiles per region.
    int tiles_per_region = 255;
    /// Celling height.
    int ceiling_height = 8;

    /// Size of a single tile.
    float tile_size = 1.0f;

    /// Flag for whether or not the map needs an update.
    bool dirty = true;

    // Build the regions/tiles.
    void _build_regions();

    // Build the tiles.
    void _build_tiles(const godot::Vector2& region_id);

    // Update the tile neighbors.
    void _update_tile_neighbors();

    // Builds the region meshes.
    void _build_region_meshes();

    // Builds the region mesh edges.
    void _build_region_mesh_edges();

    // Create a collision mesh from a given mesh.
    void _create_collision(godot::StaticBody*& collision_body, godot::CollisionShape*& collision_shape, godot::Ref<godot::Mesh> mesh, const godot::Transform& transform);

    // Create the collisions for a region.
    void _create_region_collision(const godot::Vector2& region_id);

    // Create the edge collisions for a region.
    void _create_region_edge_collision(const godot::Vector2& region_id);

    // Updates the visibility of the node.
    void _update_visibility();

    // Updates the grid transformation.
    void _update_transform();

    // Updates the material for the terrain.
    void _update_material();

protected:
    //! Handles notifications to the node.
    //! \param p_what
    void _notification(int p_what);

    //! Handles entering the world.
    //! \param world
    void enter_world(godot::World &w);

    //! Handles exiting the world.
    void exit_world();

    //! Handles the process event.
    void _process();

public:
    //! Constructor.
    DungeonMap();

    //! Destructor.
    ~DungeonMap();

    //! Builds the dungeon map mesh.
    void build();

    //! Clears the generated map.
    void clear();

    // Attempts to return a region based on the given coordinates/id.
    Region* find_region(const godot::Vector2& region_id);
    // Attempts to return a tile based on the given coordinates/id.
    Tile* find_tile(const godot::Vector2& tile_id);

    // Returns a random map location (based on the initial seed).
    godot::Vector2 get_random_map_location();

    // Returns the "snapped" tile position given a position.
    godot::Vector2 get_tile_position(const godot::Vector2& position);

    // Checks if a position is valid.
    bool is_valid_position(const godot::Vector2& position);

    // Set the map image color for a specific tile.
    void set_map_tile_color(const godot::Vector2& tile_id, godot::Color color);

    // Returns an array of the region meshes.
    godot::Array get_meshes();

    // Returns an array of the region edge meshes.
    godot::Array get_edge_meshes();

    // Marks the terrain as dirty.
    inline void mark_dirty() { dirty = true; }
    // Check if the terrain is dirty.
    inline bool is_dirty() { return dirty; }

    // Sets the size of the region.
    inline void set_region_size(int size) { mark_dirty(); region_size = size; }
    // Returns the size of the region.
    inline int get_region_size() const { return region_size; }
    // Sets the tiles per region.
    inline void set_tiles_per_region(int tiles) { mark_dirty(); tiles_per_region = tiles; }
    // Returns the tile per region.
    inline int get_tiles_per_region() const { return tiles_per_region; }

    //! Sets the tile size.
    //! \param size
    inline void set_tile_size(float size) { tile_size = size; }

    //! Returns the tile size.
    //! \return float
    inline float get_tile_size() const { return tile_size; }

    // Set the ceiling height.
    inline void set_ceiling_height(int height) { ceiling_height = height; }
    // Returns the ceiling height.
    inline int get_ceiling_height() const { return ceiling_height; }

    // Returns all regions in the map.
    inline const std::unordered_map<std::string, Region*>& get_regions() const { return regions; }
    // Returns all tiles in the map.
    inline const std::unordered_map<std::string, Tile*>& get_tiles() const { return tiles; }

    //! Sets the map image.
    //! \param image
    inline void set_map_image(godot::Ref<godot::Image> image) { map_image = image; }

    //! Sets the surface tool.
    //! \param tool
    inline void set_surface_tool(godot::Ref<godot::SurfaceTool> tool) { surfaceTool = tool; }

    //! Returns a string from a given vector.
    //! \param vec
    //! \return std::string
    inline std::string get_id(const godot::Vector2& vec) {
        return godot::String(vec).alloc_c_string();
    }

    //! Registers the methods for this class.
    static void _register_methods();
};
