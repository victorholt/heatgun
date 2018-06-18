extends Button

func _ready():
	pass


func _on_StartGameBtn_button_up():
	get_tree().change_scene('res://scenes/World.tscn')
