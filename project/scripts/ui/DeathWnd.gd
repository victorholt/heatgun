extends Control

var pause_menu = null

func _ready():
	hide()
	pause_menu = get_tree().get_root().get_node('Node/Pause')
	
func show_window():
	show()
	pause_menu.pause(true, false)
	pause_menu.disable_input = true