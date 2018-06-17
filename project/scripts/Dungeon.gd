extends Spatial

onready var giProbe = get_parent().get_node("GIProbe")
# onready var lightMap = get_parent().get_node("BakedLightmap")

onready var map_gen = preload("res://bin/map_generator.gdns").new()
onready var dungeon_map = preload("res://bin/dungeon_map.gdns").new()

func _ready():
	# Add our dungeon map and set the heightmap.
	add_child(dungeon_map)
	dungeon_map.set_owner(self)
	
	randomize()
	var sd = randi()
	map_gen.seed = sd
	map_gen.map_size = 256
	map_gen.max_floors = 500
	map_gen.generate()
	
	dungeon_map.show()
	
	dungeon_map.set_region_size(4)
	dungeon_map.set_tiles_per_region(64)
	dungeon_map.set_tile_size(1.0)
	dungeon_map.set_ceiling_height(4.0)
	dungeon_map.set_map_image(map_gen.get_map_image())	
	# dungeon_map.set_surface_tool(SurfaceTool.new())
	dungeon_map.build()	
	
	# Because the .generate_normals is broken in gdnative... try in gdscript.
	# ... ugh... still broken, so use custom module!
	
	#var meshArr = dungeon_map.get_meshes()
	#for mesh in meshArr:
	#	surface_tool.clear()
	#	surface_tool.create_from(mesh, 0)
	#	surface_tool.generate_normals()
	#	surface_tool.commit(mesh)
		
	#var meshArr = dungeon_map.get_edge_meshes()
	#for mesh in meshArr:
	#	var surface_tool = SurfaceTool.new()
	#	surface_tool.create_from(mesh, 0)
	#	surface_tool.generate_normals()
	#	surface_tool.commit(mesh)
	
	get_parent().get_node("Control").get_node("MiniMap").set_texture(map_gen.get_map_texture())
	
	# Add random blue crystals.
	_add_random_objects("res://scenes/Crystal.tscn", 4)
	
	# giProbe.bake()
	
	# Bake the GI probe.
	# get_parent().get_node("GIProbe").bake(dungeon_map)	
	# get_parent().get_node("BakedLightmap").bake()	
	
#func _process(delta):
#	pass
	
func _add_random_objects(resPath, amount):
	for i in amount:
		# Find random positions to place the crystals.
		var obj = load(resPath).instance()
		obj.set_transform(dungeon_map.get_global_transform())
		
		var map_loc = dungeon_map.get_random_map_location()
		obj.set_translation(Vector3(map_loc.x, 0.0, map_loc.y))
		obj.set_scale(Vector3(5.0, 5.0, 5.0))
		dungeon_map.add_child(obj)
		obj.set_owner(dungeon_map)
		obj.show()
		
