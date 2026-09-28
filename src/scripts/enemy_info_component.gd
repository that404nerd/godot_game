extends Node3D

@onready var movement_state_label: Label3D = $"Movement State Label"
@onready var weapon_state_label: Label3D = $"Weapon State Label"

@onready var movement_state_machine: MovementStateMachine = $"../MovementStateMachine"
@onready var weapon_state_machine: WeaponStateMachine = $"../WeaponStateMachine"

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	movement_state_label.text = "Movement State: " + movement_state_machine.get_current_state_name()
	weapon_state_label.text = "Weapon State: " + weapon_state_machine.get_current_state_name()
