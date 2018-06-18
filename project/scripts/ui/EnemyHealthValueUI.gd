extends HBoxContainer

var player = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	$Label.text = ''
	$Value.text = ''
	$Label.set('custom_colors/font_color', Color(0.68, 0.161, 0.31))
	
func _physics_process(delta):	
	if player:
		if player.current_attack_target:
			$Label.text = player.current_attack_target.npc_name
			$Value.text = String(player.current_attack_target.health)
		else:
			$Label.text = ''
			$Value.text = ''
			
		#if (player_weapon.current_weapon_temp > 50):
		#	$Value.set('custom_colors/font_color', Color(1.0, 1.0, 0.0))
		#if (player_weapon.current_weapon_temp > 100):
		#	$Value.set('custom_colors/font_color', Color(1.0, 0.0, 0.0))			
