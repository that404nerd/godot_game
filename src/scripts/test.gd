extends Node3D

@export var physical_bone_simulator_3d: PhysicalBoneSimulator3D
@export var skeleton_3d: Skeleton3D

func _ready() -> void:
	for boneNo in range(0, physical_bone_simulator_3d.get_children().size()):
		# Get the required bone (i.e. child) from the physical bone simulator parent
		var bone: PhysicalBone3D = physical_bone_simulator_3d.get_child(boneNo)
		
		# Get the required collider from the bone (which is always at index 0 by default)
		var collider: CollisionShape3D = bone.get_child(0)
		
		var bone_attachment: BoneAttachment3D = BoneAttachment3D.new()
		var bone_area_collider := collider.duplicate()
		
		bone_attachment.external_skeleton = skeleton_3d.get_path()
		bone_attachment.bone_name = bone.name
		bone_attachment.transform = bone.transform
		bone_attachment.bone_idx = bone.get_bone_id()
		
		bone_area_collider.shape = collider.shape
		bone_area_collider.transform = bone.transform
		
		bone_attachment.add_child(bone_area_collider)
		
		add_child(bone_attachment)
