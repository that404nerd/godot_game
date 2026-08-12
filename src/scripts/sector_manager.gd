extends Node

@export var map: FuncGodotMap

func _ready() -> void:
	var cover_entites: Array[Node] = map.find_children(("*func_cover_entity"))
	for entity in cover_entites:
		var entityMesh: MeshInstance3D = entity.get_child(0) as MeshInstance3D
		var material: BaseMaterial3D = entityMesh.mesh.surface_get_material(0)
		material.albedo_color = Color(1.0, 0.0, 0.0, 1.0)
