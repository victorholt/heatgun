extends HBoxContainer

var player = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	$Label.text = 'Crystals Left:'
	$Value.text = ''
	$Label.set('custom_colors/font_color', Color(0.04, 0.9, 1.0))
	
func _physics_process(delta):	
	if player:
		$Value.text = String(15 - player.crystals)
