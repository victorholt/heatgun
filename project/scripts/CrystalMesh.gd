extends MeshInstance

onready var mat = get_parent().get_node("MeshInstance").get_surface_material(0)
var energy_level = 0.0
var energy_level_dir = 1.0

func _ready():
	pass
	
func _physics_process(delta):
	mat.emission_energy = sin(energy_level)	
	energy_level += (0.005 * energy_level_dir)
	
	if energy_level > 2.0:
		energy_level_dir = -1.0
	elif energy_level < -2.0:
		energy_level_dir = 1.0
