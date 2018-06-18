extends Navigation

onready var dungeon_map = get_parent().get_node("DungeonMap")

func _ready():
	dungeon_map.add_navigation_meshes(self)
