extends StaticBody

signal on_hit

func _ready():
	set_meta('valid_hit_target', true)
	connect('on_hit', self, '_on_hit')
	
func _on_hit():
	pass
	# var mat = get_parent().get_node("MeshInstance").get_surface_material(0)
	# mat.emission_energy = 5.0
