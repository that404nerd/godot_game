@tool
extends Node3D

@export var speed := 1.0

var target_transform: Transform3D
var offset_transform: Transform3D

func _func_godot_apply_properties(entity_props: Dictionary) -> void:
	rotation = entity_props["rotation"] as Vector3
	scale = entity_props["scale"] as Vector3
	
	print(entity_props)
	
func _process(delta: float) -> void:
	pass
	
func use() -> void:
	play_motion()

func play_motion() -> void:
	target_transform = offset_transform
