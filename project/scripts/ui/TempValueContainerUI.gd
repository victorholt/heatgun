extends HBoxContainer

var player = null
var player_weapon = null

func _ready():
	$Label.text = 'Gun Temp:'
	$Value.text = '0'
	
	player = get_tree().get_root().get_node('Node/Player')
	if player:
		player_weapon = player.weapon
	
func _physics_process(delta):
	if player_weapon:
		$Value.text = String(player_weapon.current_weapon_temp)
		if (player_weapon.current_weapon_temp > 50):
			$Value.set('custom_colors/font_color', Color(1.0, 1.0, 0.0))
		if (player_weapon.current_weapon_temp > 100):
			$Value.set('custom_colors/font_color', Color(1.0, 0.0, 0.0))
			#$Value.set('custom_colors/font_color', Color(0.68, 0.161, 0.31))
	