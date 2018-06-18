extends DungeonMap

onready var mainThemeSound = $MainTheme

func _ready():
	# adding candy
	_add_random_objects('res://scenes/Crystal.tscn', 15)
	
	# spawn enemies
	_add_random_npc('res://scenes/IceNPC.tscn', 18)
	
	# re-bake the beans because we added so mo stuff!
	get_parent().get_node('GIProbe').bake()
	
	# play music.
	mainThemeSound.connect('finished', self, '_on_music_finished')
	mainThemeSound.play()

func _add_random_objects(resPath, amount):
	for i in amount:
		# Find random positions to place the object.
		var obj = load(resPath).instance()
		obj.set_transform(get_global_transform())
		
		var map_loc = get_random_map_location()
		obj.set_translation(Vector3(map_loc.x, 0.0, map_loc.y))
		obj.set_scale(Vector3(5.0, 5.0, 5.0))
		obj.get_node('MeshInstance').energy_level = rand_range(-1.0, 1.0)
		add_child(obj)		
		obj.set_owner(self)
		
func _add_random_npc(resPath, amount):
	for i in amount:
		# Find random positions to place the npc.
		var obj = load(resPath).instance()
		obj.get_node('Body').set_transform(get_global_transform())
		
		var map_loc = get_random_map_location()
		obj.get_node('Body').set_translation(Vector3(map_loc.x, 0.0, map_loc.y))
		add_child(obj)
		obj.set_owner(self)
		
func _on_music_finished():
	mainThemeSound.play(0.0)
