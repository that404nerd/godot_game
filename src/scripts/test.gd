extends Node3D

@onready var animation_tree: AnimationTree = $"../AnimationTree"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	animation_tree.connect("animation_finished", _on_anim_finished)
	
func _on_anim_finished(anim_name: StringName) -> void:
	print(anim_name)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
