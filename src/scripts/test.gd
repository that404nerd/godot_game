extends Node3D
@onready var physical_bone_simulator_3d: PhysicalBoneSimulator3D = $"../PhysicalBoneSimulator3D"

func _ready() -> void:
	physical_bone_simulator_3d.physical_bones_start_simulation()	
	pass
