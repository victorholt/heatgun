extends TextureRect

onready var dungeon_map = null

func _ready():
	dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')
	
	if dungeon_map:
		texture = dungeon_map.get_map_texture()
		
func _input(event):
	if Input.is_action_just_released('Map'):
		if is_visible_in_tree():
			hide()
		else:
			show()
	