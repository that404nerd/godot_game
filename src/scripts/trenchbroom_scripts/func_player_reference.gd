extends Node3D

@onready var player_mesh: MeshInstance3D = $player_mesh

func _ready() -> void:
	if Engine.is_editor_hint() or !Engine.is_editor_hint():
		visible = false
		player_mesh.hide()
