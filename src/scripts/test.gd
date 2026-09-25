extends Node3D

@onready var animation_tree: AnimationTree = $"../AnimationTree"
@onready var anim_sm: AnimationNodeStateMachinePlayback = animation_tree["parameters/UpperBodyStateMachine/playback"]
@onready var anim_sm2: AnimationNodeStateMachinePlayback = animation_tree["parameters/LowerBodyStateMachine/playback"]

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	anim_sm.connect("state_finished", _on_state_finished)
	anim_sm2.connect("state_finished", _on_state_finished2)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _test() -> void:
	print("test")
	
func _on_state_finished(state: StringName) -> void:
	print(state)

func _on_state_finished2(state: StringName) -> void:
	#print(state)
	pass
	
