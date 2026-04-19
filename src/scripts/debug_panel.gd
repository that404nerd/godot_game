extends PanelContainer

@export var player: Player
@onready var propertyContainer : VBoxContainer = %PropertyContainer
@onready var playerStateMachine : PlayerStateMachine = %PlayerStateMachine

# We use this to add a new label in the property container
var property : Label
var frames_per_second : String

func _ready() -> void:
	visible = true
	add_debug_property("FPS", frames_per_second)
	
func _process(_delta: float) -> void:
	if visible == true:
		frames_per_second = str(Engine.get_frames_per_second())
		property.text = property.name + ": " + frames_per_second
	print(playerStateMachine.get_current_state())
	
func _unhandled_input(_event: InputEvent) -> void:
	if Input.is_action_just_pressed("toggle_debug_panel"):
		visible = !visible

func add_debug_property(title: String, value):
	property = Label.new()
	propertyContainer.add_child(property)
	property.name = title
	property.text = property.name + value
