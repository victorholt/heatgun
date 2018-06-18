extends Button

func _ready():
	pass

func _on_Resume_pressed():
	get_tree().get_root().get_node('Node/Pause').pause(false, false)
