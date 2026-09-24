extends Node3D

@onready var rifle: Node3D = $Skeleton3D/Rifle
@onready var skeleton_3d: Skeleton3D = $Skeleton3D

func _ready() -> void:
	pass # Replace with function body.


func _process(delta: float) -> void:
	var bone_idx: int = skeleton_3d.find_bone("mixamorig_RightHand")
	var bone_trf: Transform3D = skeleton_3d.get_bone_global_pose(bone_idx)
	#rifle.set_transform(bone_trf)
