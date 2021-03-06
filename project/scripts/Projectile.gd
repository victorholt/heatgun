extends RigidBody

onready var projectile_exp = preload('res://scenes/ProjectileEnd.tscn')

var timer = 0
var damage = 10
var exp_state = null

var player = null

# Explore using godot's timer...
var projectile_expire_time = 30

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	exp_state = projectile_exp.instance()
	exp_state.hide()
	set_meta('projectile', true)
	$sfx.play(0.0)

func _physics_process(delta):
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
			# valid_hit_target IS a meta value set by the custom
			# dungeon_map module compiled with the engine. NOT
			# STANDARD!
			if c.has_meta('projectile'):
				damage = 0
				hide()
				return
			
			if c.has_meta('valid_hit_target'):
				linear_velocity = Vector3()
				gravity_scale = 0.0						
				
				#if player:
				#	player.update_current_attack_target(null)
				
				# Hide the projectile so we can display the death particle.
				$CollisionShape/MeshInstance.hide()
				exp_state.show()
				c.emit_signal('on_hit')
								
				if c.has_meta("is_npc"):
					c.get_parent().hit(damage)
					damage = 0
				return
