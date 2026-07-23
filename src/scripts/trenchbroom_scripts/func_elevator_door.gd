@tool
class_name ElevatorDoor extends AnimatableBody3D

@export_range(1.0, 10.0, 1.0) var move_speed: float
@export var move_distance: float = 2.0
@export var move_direction: Vector3 = Vector3(0.0, 1.0, 0.0)

var start_position: Vector3
var end_position: Vector3
var door_tween: Tween

func _func_godot_apply_properties(entity_props: Dictionary) -> void:
	move_direction = entity_props["move_direction"] as Vector3
	move_speed = entity_props["move_speed"] as float
	move_distance = entity_props["move_distance"] as float
	
func _ready() -> void:
	if not Engine.is_editor_hint():
		start_position = global_position
		end_position = start_position + (move_direction.normalized() * move_distance)
		_start_movement()
		
func _start_movement() -> void:
	door_tween = create_tween()
	#door_tween.tween_property(self, "global_position", end_position, move_speed)
