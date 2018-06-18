extends AudioStreamPlayer

func _ready():
	connect('finished', self, '_on_music_finished')
	
func _on_music_finished():
	play(0.0)
