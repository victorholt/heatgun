extends DungeonMap

func _ready():
	# adding candy
	_add_random_objects("res://scenes/Crystal.tscn", 15)
	
	# re-bake the beans because we added so mo stuff!
	get_parent().get_node('GIProbe').bake()

func _add_random_objects(resPath, amount):
	for i in amount:
		# Find random positions to place the crystals.
		var obj = load(resPath).instance()
		obj.set_transform(.get_global_transform())
		
		var map_loc = .get_random_map_location()
		obj.set_translation(Vector3(map_loc.x, 0.0, map_loc.y))
		obj.set_scale(Vector3(5.0, 5.0, 5.0))
		.add_child(obj)
		obj.set_owner(self)
		obj.show()
		obj.get_node("MeshInstance").energy_level = rand_range(-1.0, 1.0)
