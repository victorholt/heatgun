extends Navigation

onready var dungeon_map = get_parent().get_node("DungeonMap")

func _ready():
	update(null)
	
func update(dm):
	dungeon_map = dm
	if dungeon_map:
		dungeon_map.remove_navigation_meshes(self)
		dungeon_map.add_navigation_meshes(self)
