extends HBoxContainer

var player = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	$Label.text = 'Health:'
	$Value.text = ''
	
func _physics_process(delta):	
	if player:
		$Value.text = String(player.health)			
		if (player.health < 10):
			$Value.set('custom_colors/font_color', Color(1.0, 1.0, 0.0))
		if (player.health < 7):
			$Value.set('custom_colors/font_color', Color(1.0, 0.0, 0.0))
		if (player.health > 10):
			$Value.set('custom_colors/font_color', Color(1.0, 1.0, 1.0))
