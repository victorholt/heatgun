extends HBoxContainer

var player = null
var dungeon_map = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	dungeon_map = get_tree().get_root().get_node('Node/DungeonMap')
	
	$Label.text = 'Crystals Left:'
	$Value.text = ''
	$Label.set('custom_colors/font_color', Color(0.04, 0.9, 1.0))
	
func _physics_process(delta):	
	if player:
		var total_left = dungeon_map.num_objects_on_map - player.crystals		
		$Value.text = String(total_left)
		
		if total_left == 0:
			get_tree().get_root().get_node('Node/NextLevel').show_window()
