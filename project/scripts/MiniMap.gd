extends TextureRect

onready var map_gen = preload("res://bin/map_generator.gdns").new()

func _ready():
	randomize()
	var sd = randi()
	map_gen.seed = sd
	map_gen.map_size = 128
	map_gen.generate()
	texture = map_gen.get_map_texture()