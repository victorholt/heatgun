extends Spatial

var map_gen = preload("res://bin/map_generator.gdns").new()
var dungeon_map = preload("res://bin/dungeon_map.gdns").new()
var surface_tool = SurfaceTool.new()

func _ready():
	print("Building dungeon in editor")
	
	# Add our dungeon map and set the heightmap.
	add_child(dungeon_map)
	dungeon_map.set_owner(self)
	
	randomize()
	var sd = randi()
	map_gen.seed = sd
	map_gen.map_size = 128
	map_gen.generate()
	
	dungeon_map.show()
	# dungeon_map.set_translation(get_translation())
	
	dungeon_map.set_region_size(2)
	dungeon_map.set_tiles_per_region(64)
	dungeon_map.set_tile_size(1.0)
	dungeon_map.set_ceiling_height(4.0)
	dungeon_map.set_map_image(map_gen.get_map_image())	
	dungeon_map.set_surface_tool(surface_tool)
	dungeon_map.build()	
	
	# Because the .generate_normals is broken in gdnative... try in gdscript.
	var meshArr = dungeon_map.get_meshes()
	for mesh in meshArr:
		surface_tool.clear()
		surface_tool.create_from(mesh, 0)
		surface_tool.generate_normals()
		surface_tool.commit(mesh)
