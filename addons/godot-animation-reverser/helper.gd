extends RefCounted

static func default_name(old_name: String) -> String:
	return "%s-reversed" % old_name

static func build_new_name(anim_player: AnimationPlayer, full_old_name: String, raw_anim_name: String) -> String:
	var to_return := raw_anim_name

	if "backwards" in raw_anim_name:
		to_return = raw_anim_name.replace("backwards", "forwards")
	elif "forwards" in raw_anim_name:
		to_return = raw_anim_name.replace("forwards", "backwards")

	var lib_prefix := ""
	if "/" in full_old_name:
		lib_prefix = full_old_name.get_slice("/", 0) + "/"

	if anim_player.has_animation(lib_prefix + to_return):
		return default_name(raw_anim_name)

	return to_return

static func reverse(animation_player: AnimationPlayer, full_anim_name: String, new_name: String = "") -> Animation:
	if not animation_player.has_animation(full_anim_name):
		push_error("Animation '%s' could not be found" % full_anim_name)
		return null

	var anim: Animation = animation_player.get_animation(full_anim_name)
	var new_anim: Animation = anim.duplicate(true)
	var length: float = anim.length

	for track_idx in range(new_anim.get_track_count()):
		var key_items := []
		var key_count := anim.track_get_key_count(track_idx)

		# 1. Read keys in reverse order and compute flipped timestamps
		for key_idx in range(key_count - 1, -1, -1):
			key_items.append([
				length - anim.track_get_key_time(track_idx, key_idx),
				anim.track_get_key_value(track_idx, key_idx),
				anim.track_get_key_transition(track_idx, key_idx),
			])

		# 2. Clear old keys from back to front so Godot 4 doesn't auto-sort mid-edit
		for key_idx in range(key_count - 1, -1, -1):
			new_anim.track_remove_key(track_idx, key_idx)

		# 3. Insert reversed keys cleanly
		for key_item in key_items:
			new_anim.track_insert_key(track_idx, key_item[0], key_item[1], key_item[2])

	# Handle Godot 4 AnimationLibrary ("lib_name/anim_name" or default "")
	var lib_name := ""
	var raw_anim_name := full_anim_name
	if "/" in full_anim_name:
		lib_name = full_anim_name.get_slice("/", 0)
		raw_anim_name = full_anim_name.get_slice("/", 1)

	if new_name.is_empty():
		new_name = build_new_name(animation_player, full_anim_name, raw_anim_name)

	var library: AnimationLibrary = animation_player.get_animation_library(lib_name)
	if not library:
		push_error("AnimationLibrary '%s' not found!" % lib_name)
		return null

	# Note: If the AnimationLibrary is imported directly from a .glb/.blend, make sure it's local/editable
	if library.has_animation(new_name):
		library.remove_animation(new_name)
	library.add_animation(new_name, new_anim)

	var final_assigned_name := (lib_name + "/" + new_name) if not lib_name.is_empty() else new_name
	animation_player.assigned_animation = final_assigned_name
	print("Successfully created reversed animation: ", final_assigned_name)

	return new_anim

static func reverse_current(animation_player: AnimationPlayer, anim_name: String = "", new_name: String = "") -> Animation:
	if not animation_player:
		push_error("No AnimationPlayer passed!")
		return null

	if anim_name.is_empty():
		anim_name = animation_player.assigned_animation

	if anim_name.is_empty():
		push_error('No animation selected in the AnimationPlayer editor dock!')
		return null

	return reverse(animation_player, anim_name, new_name)
