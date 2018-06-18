extends "res://scripts/BaseNPC.gd"

onready var projectile = preload("res://scenes/NPCProjectile.tscn")

var npc_speed = 0.5
var can_fire = false
var fire_speed = 45.0
var npc_mat

# Handles any initialization for the npc.
func _on_ready():
	npc_name = 'Ice Guard'
	
	# Duplicate our material so we can have hit effects.
	npc_mat = model.get_surface_material(0).duplicate()
	model.set_surface_material(0, npc_mat)
	
	$FireCooldown.stop()
	$FireCooldown.connect('timeout', self, '_on_weapon_fire_cooldown_timeout')
	
	$HitMaterialCooldown.stop()
	$HitMaterialCooldown.connect('timeout', self, '_on_hit_cooldown_timeout')

# Handles NPC patrolling on the map.
func _on_patrol(delta):
	# Check if we're colliding with something before we move.
	if !$HitMaterialCooldown.is_stopped():
		npc_mat.emission = Color(1.0, 0.0, 0.0)
		npc_mat.emission_energy = 2.0
	
	# Move to the player if they're close.
	if player_in_long_view() && !player_in_view():
		if player && path_to_player.size() == 0:	
			path_to_player = navigation.get_simple_path(body.translation, player.translation, false)			
			if path_to_player.size() == 0:
				return false
			path_index = path_to_player.size() - 1
			current_move_target = path_to_player[path_index]
		
		if body.translation.distance_to(current_move_target) < 1.0:
			path_index = 0
			current_move_target = Vector3()
			path_to_player = PoolVector3Array()
			return
		
		if path_to_player.size() > 0 && path_index > 0:		
			var next_pos = Vector3()
			next_pos = body.translation.linear_interpolate(current_move_target, npc_speed * delta)
			path_index -= 1
			current_move_target = path_to_player[path_index]
			
			# Unmark our last location.
			var last_pos = dungeon_map.get_tile_position(Vector2(last_position.x, last_position.z))
			dungeon_map.mark_tile_occupied(last_pos, false)
												
			var tile_pos = dungeon_map.get_tile_position(Vector2(body.translation.x, body.translation.z))
			var next_tile_pos = dungeon_map.get_tile_position(Vector2(next_pos.x, next_pos.z))
			
			if !dungeon_map.is_valid_position(next_tile_pos):
				reset_movement()
				return
			if dungeon_map.is_tile_occupied(next_tile_pos):
				reset_movement()
				return
			
			body.translation = next_pos
			body.translation.y = 0.4
			
			dungeon_map.mark_tile_occupied(next_tile_pos, true)
			
			dungeon_map.set_map_tile_color(last_position, Color(1, 1, 1))
			last_position = next_pos
			update_map_loc = true			

# Handles the attack action.
func _on_attack_action():
	if !$FireCooldown.is_stopped():
		return
		
	$FireCooldown.start()
	var proj = projectile.instance()
	
	proj.set_transform(body.get_global_transform())
	proj.translation.y += 0.5
	get_parent().add_child(proj)
	
	#var proj_vel = player.body.get_global_transform().basis[2].normalized() * Vector3(1, 0, 1)
	var dir = player.transform.origin - body.translation
	# dir = dir.normalized()
	dir.y += 0.5
	
	var vel = dir.length() * 10.0
	
	proj.set_linear_velocity(fire_speed * dir.normalized())
	proj.add_collision_exception_with(body)
	
# Handles the hit action.
func _on_hit_action(damage):
	$HitMaterialCooldown.start()
	
# Handles the death of the npc.
func _on_death():
	return
	
func _on_reset_cooldowns():
	$FireCooldown.stop()
	$HitMaterialCooldown.stop()
	
func _on_weapon_fire_cooldown_timeout():
	$FireCooldown.stop()
	return
	
func _on_hit_cooldown_timeout():	
	$HitMaterialCooldown.stop()
	npc_mat.emission = Color(1.0, 1.0, 1.0)
	npc_mat.emission_energy = 0.75
	return
