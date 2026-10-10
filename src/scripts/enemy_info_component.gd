extends Node3D

@onready var movement_state_label: Label3D = $"Movement State Label"
@onready var weapon_state_label: Label3D = $"Weapon State Label"
@onready var health_component: HealthComponent = $"../HealthComponent"

@onready var movement_state_machine: MovementStateMachine = $"../MovementStateMachine"
@onready var weapon_state_machine: WeaponStateMachine = $"../WeaponStateMachine"

func _ready():
	health_component.death.connect(_on_enemy_dead)
	
func _on_enemy_dead():
	set_visible(false)
	queue_free()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	movement_state_label.text = "Movement State: " + movement_state_machine.get_current_state_name()
	
	if(weapon_state_machine):
		weapon_state_label.text = "Weapon State: " + weapon_state_machine.get_current_state_name()
