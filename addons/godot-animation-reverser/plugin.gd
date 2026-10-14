@tool
extends EditorPlugin

var enabled := false
var edited_animation_player: AnimationPlayer
const Helper = preload("res://addons/godot-animation-reverser/helper.gd")

func _handles(object: Object) -> bool:
	var does_handle := object is AnimationPlayer

	if does_handle:
		if not enabled:
			enabled = true
			add_tool_menu_item("Reverse animation", activate)
	elif enabled:
		enabled = false
		remove_tool_menu_item("Reverse animation")

	return does_handle

func _edit(object: Object) -> void:
	edited_animation_player = object as AnimationPlayer

func activate() -> void:
	var selected_anim := _get_editor_selected_animation()
	Helper.reverse_current(edited_animation_player, selected_anim)

func _get_editor_selected_animation() -> String:
	if edited_animation_player and not edited_animation_player.assigned_animation.is_empty():
		return edited_animation_player.assigned_animation
	return ""

func _exit_tree() -> void:
	if enabled:
		remove_tool_menu_item("Reverse animation")
		enabled = false
