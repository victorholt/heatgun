extends Button

onready var dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')

func _ready():
	pass

func _on_NextLevelBtn_pressed():
	var new_size = dungeon_map.dungeon_map_size
	var num_enemies = dungeon_map.num_enemies_on_map
	var num_objects = dungeon_map.num_objects_on_map
	
	new_size += 2
	num_enemies += 2
	num_objects += 3
	
	if new_size > 10:
		new_size = 10
	if num_enemies > 20:
		num_enemies = 20
	if num_objects > 15:
		num_objects = 15
	
	dungeon_map.new_map(new_size, num_enemies, num_objects)	