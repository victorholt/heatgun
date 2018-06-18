extends StaticBody

signal on_hit

var player = null
var is_changed = false
var dungeon_map = null

func _ready():	
	dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')
	player = get_tree().get_root().get_node('Node/Player')
	set_meta('valid_hit_target', true)
	connect('on_hit', self, '_on_hit')	
	
func _physics_process(delta):
	if is_changed:
		var pos = get_parent().get_translation()
		dungeon_map.set_map_tile_color(pos, Color(0.04, 0.9, 1.0))
	
func _on_hit():
	if is_changed:
		return
	
	var mat = get_parent().get_node("MeshInstance").get_surface_material(0)
	mat.emission = Color(1.0, 0.0, 0.0)
	player.crystals += 1
	player.add_health(5)
	is_changed = true
	
	var light = get_parent().get_node("MeshInstance/OmniLight")
	light.light_color = Color(1.0, 0.0, 0.0)
	
	# Update our map so we know where we have been.
	var pos = get_parent().get_translation()
	#dungeon_map.set_tile_color(pos, Color(0.04, 0.9, 1.0))	
	dungeon_map.set_map_tile_color(pos, Color(0.04, 0.9, 1.0))
	
