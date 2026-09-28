@tool
extends Node3D

@export var dict: Dictionary[StringName, StringName]

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	dict["Hello"] = StringName("")
	dict["Hello2"] = StringName("")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
