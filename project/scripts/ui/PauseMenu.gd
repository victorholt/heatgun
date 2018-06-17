extends Control

var camera = null

# This is were a STATE MACHINE is pretty handy...
var is_paused = false

func _ready():
	hide()
	camera = get_tree().get_root().get_node('Node/Camera')
	
func _input(event):
	if Input.is_action_just_pressed('PauseMenu'):
		is_paused = !is_paused
	if is_paused:
		if camera:
			camera.set_enabled(false)
			Input.set_mouse_mode(3)
		show()
	else:
		if camera:
			camera.set_enabled(true)			
		hide()	