extends MeshInstance

signal fire_weapon

# Best to have some sort of state machine for the entire game...
var pauseMenu = null

# Weapon type and how much temp it takes to fire 1 shot
var weapon_type = {
	'rapid_fire': {
		'name': 'Quick Fire',
		'temp': 4,
		'fire_time': 0.15,
		'temp_cooldown': 0.3,
		'damage': 3,
	},	
	
	'power_gun': {
		'name': 'Hot Sauce',
		'temp': 20,
		'fire_time': 0.65,
		'temp_cooldown': 0.75,
		'damage': 15
	},
}

# Current temperature of the weapon
var current_weapon_temp = 0
# Current weapon selected
var current_weapon = 'rapid_fire'

func _ready():
	pauseMenu = get_tree().get_root().get_node("Node/Pause")
	
	connect('fire_weapon', self, '_on_fire_weapon')
	$FireCooldown.connect('timeout', self, '_on_weapon_fire_cooldown_timeout')
	$TempCooldown.connect('timeout', self, '_on_weapon_temp_cooldown_timeout')
	
	$FireCooldown.stop()
	$TempCooldown.stop()
	
func _process(delta):
	if pauseMenu && pauseMenu.is_paused:
		$FireCooldown.set_paused(true)
		$TempCooldown.set_paused(true)
	else:
		$FireCooldown.set_paused(false)
		$TempCooldown.set_paused(false)
	
func can_fire():
	return $FireCooldown.is_stopped() && current_weapon_temp < 200
	
func get_damage():
	return weapon_type[current_weapon]['damage']

func _on_fire_weapon():
	if !can_fire():
		return
	
	var weapon_info = weapon_type[current_weapon]
	current_weapon_temp += weapon_info['temp']
	
	$FireCooldown.start(weapon_info['fire_time'])
	$TempCooldown.start(weapon_info['temp_cooldown'])

func _update_temp():
	if pauseMenu && pauseMenu.is_paused:
		return
		
	if current_weapon_temp > 0:
		current_weapon_temp -= 5
	if current_weapon_temp < 0:
		current_weapon_temp = 0
	
func _on_weapon_fire_cooldown_timeout():
	$FireCooldown.stop()	
	
func _on_weapon_temp_cooldown_timeout():
	_update_temp()
	$FireCooldown.start()
