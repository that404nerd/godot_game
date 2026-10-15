@tool
extends Node3D

@export var damage_curve: Curve

func _ready() -> void:
	damage_curve.max_value = 10.0
