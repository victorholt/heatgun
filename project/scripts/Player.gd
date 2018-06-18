extends KinematicBody

# Our global camera.
var dungeon_map = null
onready var camera = get_parent().get_node("Camera")
onready var weapon = get_node("Weapon")
onready var weaponMuzzle = get_node("WeaponFireExit")
onready var projectile = preload("res://scenes/Projectile.tscn")
onready var body = get_node('Character')
onready var death_wnd = get_tree().get_root().get_node('Node/Death')

# Best to have some sort of state machine for the entire game...
var pauseMenu = null

# Current attack target.
var current_attack_target = null
var last_map_position = Vector3()

# Movement variables
var velocity = Vector3()
var dir = Vector3()

var health = 15
var is_dead = false
var crystals = 0

const speed = 15.0
const accel = 2.0
const deaccel = 20.0
const gravity = -10.0

func _ready():	
	set_meta("is_player", true)	
	dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')
	pauseMenu = get_tree().get_root().get_node("Node/Pause")	
	
func _physics_process(delta):
	if pauseMenu && pauseMenu.is_paused:
		return
	
	walk(delta)
	fire_weapon(delta)
	
func reset_player(dm):
	dungeon_map = dm
	current_attack_target = null
	health = 15
	crystals = 0
	is_dead = false
	var map_loc = dungeon_map.get_random_map_location()	
	translation = Vector3(map_loc.x, 1.2, map_loc.y)
	
func walk(delta):
	var dir = Vector3()
	var inputDir = Vector3()
	var aim = camera.get_global_transform().basis
	
	# Handle player movement.
	if Input.is_action_pressed('ui_up'):
		inputDir.z -= 1.0
	if Input.is_action_pressed('ui_down'):
		inputDir.z += 1.0
	if Input.is_action_pressed('ui_left'):
		inputDir.x += 1.0
	if Input.is_action_pressed('ui_right'):
		inputDir.x -= 1.0
		
	inputDir = inputDir.normalized()
	
	dir += aim.z * inputDir.z
	dir += -aim.x * inputDir.x
	dir.y = 0
	dir = dir.normalized()
	
	velocity.y += gravity * delta
	
	var tmp_vel = Vector3(velocity.x, 0, velocity.z)
	
	# Sprint?
	
	var target = dir * speed
	var cur_accel = deaccel
	
	# Check if we're still moving.
	if dir.dot(tmp_vel) > 0:
		cur_accel = accel
		
	tmp_vel = tmp_vel.linear_interpolate(target, cur_accel * delta) 
	velocity.x = tmp_vel.x
	velocity.z = tmp_vel.z
	velocity.y = 0
	
	move_and_slide(velocity, Vector3(0,1,0))
	
	if last_map_position != Vector3(0,0,0):
		dungeon_map.set_map_tile_color(last_map_position, Color(1,1,1))
	
	last_map_position = translation
	dungeon_map.set_map_tile_color(last_map_position, Color(0.68, 0.161, 0.31))
	
func hit(obj, damage):
	obj.hide()
	
	if !is_dead:
		health -= damage
	if health <= 0:
		is_dead = true
		death_wnd.show_window()
	return
	
func add_health(amount):
	health += amount
	if health > 20:
		health = 20

func fire_weapon(delta):
	if !Input.is_action_pressed('fire_weapon'):
		return	
	if !weapon.can_fire():
		return
		
	var proj = projectile.instance()
	proj.damage = weapon.get_damage()
	
	proj.set_transform(weaponMuzzle.get_global_transform())
	get_parent().add_child(proj)
	
	var proj_vel = weapon.get_global_transform().basis[2].normalized() * Vector3(-1, 0, -1)
	proj.set_linear_velocity(proj_vel * 50)
	proj.add_collision_exception_with(self)
	proj.add_collision_exception_with(weapon)
	
	weapon.emit_signal('fire_weapon')
	
func update_current_attack_target(target):
	current_attack_target = target
	

	
	
	
	
	
	
	
	
