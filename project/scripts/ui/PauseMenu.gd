extends Control

var camera = null

# This is were a STATE MACHINE is pretty handy...
var is_paused = false
var disable_input = false

func _ready():
	hide()
	camera = get_tree().get_root().get_node('Node/Camera')
	
func _input(event):
	if !disable_input && Input.is_action_just_pressed('PauseMenu'):
		is_paused = !is_paused		
	pause(is_paused, true)
	
	#if is_paused:
	#	if camera:
	#		camera.set_enabled(false)
	#		Input.set_mouse_mode(3)
	#	show()
	#else:
	#	if camera:
	#		camera.set_enabled(true)			
	#	hide()	

func pause(pause_game, show_pause_menu):
	is_paused = pause_game
	if is_paused:
		if camera:
			camera.set_enabled(false)
			Input.set_mouse_mode(3)
		if show_pause_menu && !disable_input:
			show()
		else:
			hide()
	else:
		if camera:
			camera.set_enabled(true)
			hide()
	
		