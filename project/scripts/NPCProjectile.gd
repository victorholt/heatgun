extends RigidBody

var timer = 0
var damage = 2
var exp_state = null

var pause_menu = null
var player = null
var pause_velocity = Vector3()

# Explore using godot's timer...
var projectile_expire_time = 50

func _ready():
	set_meta('projectile', true)
	pause_menu = get_tree().get_root().get_node("Node/Pause")
	$sfx.play(0.0)

func _physics_process(delta):
	if pause_menu && pause_menu.is_paused:
		if pause_velocity == Vector3():
			pause_velocity = linear_velocity
		linear_velocity = Vector3()
		mode = RigidBody.MODE_STATIC
		return
	elif pause_velocity != Vector3():
		linear_velocity = pause_velocity
		pause_velocity = Vector3()
		mode = RigidBody.MODE_RIGID
		
	_check_hit()
	
	# Check if we need to delete the projectile.
	timer += delta
	if timer > projectile_expire_time:
		timer = 0
		
		if exp_state:
			exp_state.queue_free()
		queue_free()		
	timer += 1	
	
func _check_hit():		
	if contacts_reported > 0:
		var bodies = get_colliding_bodies()
		for c in bodies:
			if c.has_meta('projectile'):
				damage = 0
				hide()
				return
				
			if c.has_meta('is_player'):
				c.hit(self, damage)
				damage = 0
				return
			
			# valid_hit_target IS a meta value set by the custom
			# dungeon_map module compiled with the engine. NOT
			# STANDARD!
			if c.has_meta('valid_hit_target'):
				linear_velocity = Vector3()
				gravity_scale = 0.0
				
				# Hide the projectile so we can display the death particle.
				$CollisionShape/MeshInstance.hide()				
				return
