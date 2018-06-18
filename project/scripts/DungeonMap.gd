extends DungeonMap

onready var mainThemeSound = $MainTheme

var num_enemies_on_map = 5
var num_objects_on_map = 5
var dungeon_map_size = 2

var enemies = Array()
var objects = Array()

func _ready():
	new_map(2, 1, 1)	
	
	# play music.
	mainThemeSound.connect('finished', self, '_on_music_finished')
	mainThemeSound.play(0.0)
	
func new_map(map_size, num_enemies, num_objects):
	clear_map_objects()
	
	randomize()
	var new_seed = randi() % 999999
	set_dungeon_seed(new_seed)
	set_region_size(map_size)
	
	clear()
	generate_map_image()	
	apply()
	
	dungeon_map_size = map_size	
	num_enemies_on_map = num_enemies
	num_objects_on_map = num_objects
	restart()
	
func restart():
	# mainThemeSound.stop()	
	clear_map_objects()	
	
	get_tree().get_root().get_node('Node/Navigation').update(self)	
	get_tree().get_root().get_node('Node/NextLevel').hide()	
	get_tree().get_root().get_node('Node/Death').hide()
	get_tree().get_root().get_node('Node/Pause').pause(false, false)
	get_tree().get_root().get_node('Node/Pause').disable_input = false	
	get_tree().get_root().get_node('Node/Player').reset_player(self)
	
	# adding candy
	_add_random_objects('res://scenes/Crystal.tscn', num_objects_on_map)
	
	# spawn enemies
	_add_random_npc('res://scenes/IceNPC.tscn', num_enemies_on_map)
	
	get_tree().get_root().get_node('Node/HUDItems/PlayerContainer/MiniMap').update_map(self)
	
	# re-bake the beans because we added so mo stuff!
	# get_parent().get_node('GIProbe').bake()	
	
func clear_map_objects():
	for obj in objects:
		obj.get_node('StaticBody').reset_map_color()
		obj.set_process(false)
		obj.set_physics_process(false)
		obj.queue_free()
	objects = Array()
		
	for enemy in enemies:
		enemy.set_process(false)
		enemy.set_physics_process(false)
		enemy.queue_free()
	enemies = Array()

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
		objects.push_back(obj)
		
func _add_random_npc(resPath, amount):
	for i in amount:
		# Find random positions to place the npc.
		var obj = load(resPath).instance()
		obj.get_node('Body').set_transform(get_global_transform())
		
		var map_loc = get_random_map_location()
		obj.get_node('Body').set_translation(Vector3(map_loc.x, 0.0, map_loc.y))
		add_child(obj)
		obj.set_owner(self)
		enemies.push_back(obj)
		
func _on_music_finished():
	mainThemeSound.play(0.0)
