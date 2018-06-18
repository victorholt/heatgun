extends HBoxContainer

var player = null

func _ready():
	player = get_tree().get_root().get_node('Node/Player')
	$Label.text = 'Health:'
	$Value.text = ''
	
func _physics_process(delta):	
	if player:
		$Value.text = String(player.health)
