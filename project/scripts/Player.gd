extends KinematicBody

# Our global camera.
onready var camera = get_parent().get_node("Camera")
onready var weapon = get_node("Weapon")
onready var weaponMuzzle = get_node("WeaponFireExit")
onready var projectile = preload("res://scenes/Projectile.tscn")

# Movement variables
var velocity = Vector3()
var dir = Vector3()

const speed = 20.0
const accel = 2.0
const deaccel = 20.0
const gravity = -10.0

func _ready():	
	var dungeon_map = get_parent().get_node("DungeonMap")	
	var map_loc = dungeon_map.get_random_map_location()
	translation = Vector3(map_loc.x, 1.2, map_loc.y)
	
func _physics_process(delta):
	walk(delta)
	fire_weapon(delta)

func _input(event):
	pass
	
func walk(delta):
	var dir = Vector3()
	var inputDir = Vector3()
	var aim = camera.get_global_transform().basis
	
	# Handle player movement.
	if Input.is_action_pressed("ui_up"):
		inputDir.z -= 1.0
	if Input.is_action_pressed("ui_down"):
		inputDir.z += 1.0
	if Input.is_action_pressed("ui_left"):
		inputDir.x += 1.0
	if Input.is_action_pressed("ui_right"):
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
	
	velocity = move_and_slide(velocity, Vector3(0,1,0))

func fire_weapon(delta):
	if !Input.is_action_just_pressed("fire_weapon"):
		return
		
	var proj = projectile.instance()
	proj.set_transform(weaponMuzzle.get_global_transform())
	get_parent().add_child(proj)
	
	var proj_vel = weapon.get_global_transform().basis[2].normalized() * Vector3(-1, 0, -1)
	proj.set_linear_velocity(proj_vel * 50)
	proj.add_collision_exception_with(self)
	proj.add_collision_exception_with(weapon)
	
	
	
	
	
	
	
	
