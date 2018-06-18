extends Button

onready var dungeon = get_tree().get_root().get_node('Node/DungeonMap')

func _ready():
	pass

func _on_RestartBtn_pressed():
	dungeon.restart()
