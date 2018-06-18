extends Node

# Base object for all npcs

# Signals
signal npc_attack
signal npc_death

# Member variables
var health = 20
var is_dead = false
var death_mesh = null
var npc_name = ''

var player = null
var dungeon_map = null
var navigation = null

var path_to_player = PoolVector3Array()
var current_move_target = Vector3()
var path_index = 0
var last_position = Vector3()
var update_map_loc = false

onready var body = get_node("Body")
onready var model = get_node("Body/Model")

# Best to have some sort of state machine for the entire game...
# Ensure we don't do anything while paused.
var pauseMenu = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')
	navigation = get_tree().get_root().get_node('Node/Navigation')
	pauseMenu = get_tree().get_root().get_node("Node/Pause")
	
	if body:
		body.set_meta("is_npc", true)
		body.set_meta('valid_hit_target', true)
		
	connect('npc_attack', self, 'attack')
	connect('npc_death', self, 'die')
	_on_ready()
	
func _physics_process(delta):
	if is_dead:
		die()
		return
		
	if player && player.is_dead:
		reset_cooldowns()
		return
	
	_check_hit()
	patrol(delta)
	
	if update_map_loc:
		if player_in_long_view():
			dungeon_map.set_map_tile_color(last_position, Color(1, 0, 0))
		else:
			dungeon_map.set_map_tile_color(last_position, Color(1, 1, 1))
	
# Patrols the map.
func patrol(delta):
	if pauseMenu && pauseMenu.is_paused:
		return
		
	if player_in_view():		
		attack()
	else:
		_on_patrol(delta)
	
# Attack an enemy.
func attack():
	if pauseMenu && pauseMenu.is_paused:
		return
	if player && player.is_dead:
		return
	_on_attack_action()
	
# Called when the npc is hit.
func hit(damage):
	if pauseMenu && pauseMenu.is_paused:
		return
	if is_dead:
		return
		
	if damage > health:
		health = 0
		is_dead = true
	else:		
		health -= damage
	
	# Terrible, but we're short on time...
	if player:
		player.update_current_attack_target(self)
		
	_on_hit_action(damage)	
	
# Kills the npc.
func die():
	body.hide()
	dungeon_map.set_map_tile_color(last_position, Color(1, 1, 1))
	
	# Terrible, but we're short on time...
	if player:
		player.update_current_attack_target(null)
		
	_on_death()
	queue_free()
	
# Reset the player cooldowns
func reset_cooldowns():
	return
	
# Resets the movement.
func reset_movement():
	path_to_player = PoolVector3Array()
	current_move_target = Vector3()
	path_index = 0
	
# Returns if the player is in view.
func player_in_view():
	if player && !player.is_dead && body.translation.distance_to(player.translation) > 15:
		path_to_player = PoolVector3Array()
		return false	
	
	# Rotate in the direction of the player.
	rotate_at_player()
		
	return true
	
# Returns if the player is in view.
func player_in_long_view():
	if player && !player.is_dead && body.translation.distance_to(player.translation) > 30:
		return false	
	
	# Rotate in the direction of the player.
	rotate_at_player()
		
	return true
	
# Rotates in the direction of the player.
func rotate_at_player():
	body.look_at(player.body.global_transform.origin, Vector3(0, 1, 0))
	
# Raycast at a target.
func raycast_at(target, delta):
	var space_state = player.get_world().direct_space_state
	var result = space_state.intersect_ray(body.get_global_transform().origin, target)
	return result
	
# Handles any initialization for the npc.
func _on_ready():
	return

# Handles NPC patrolling on the map.
func _on_patrol(delta):
	return

# Handles the attack action.
func _on_attack_action():
	return
	
# Handles the hit action.
func _on_hit_action(damage):
	return
	
# Handles the death of the npc.
func _on_death():
	return
	
func _on_reset_cooldowns():
	return
	
func _check_hit():		
	if is_dead:
		return

	if body.contacts_reported > 0:
		var bodies = body.get_colliding_bodies()
		for c in bodies:
			print("finding bodies")
			if is_dead:
				reset_movement()
				return
				
			# valid_hit_target IS a meta value set by the custom
			# dungeon_map module compiled with the engine. NOT
			# STANDARD!
			if c.has_meta('projectile'):
				print("hhit")
				body.linear_velocity = Vector3()
				body.gravity_scale = 0.0				
				hit(c.damage)
				c.damage = 0				
				return
				
