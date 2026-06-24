extends Control

@onready var weapon_manager: WeaponManager = %WeaponManager
@onready var movementStateMachine: MovementStateMachine = %MovementStateMachine
@onready var weaponStateMachine : WeaponStateMachine = %WeaponStateMachine
@onready var weaponComponent : WeaponComponent = %WeaponComponent

@onready var fps_label: Label = %FPSLabel
@onready var player_state_name_label: Label = %PlayerStateNameLabel
@onready var weapon_state_name_label: Label = %WeaponStateNameLabel
@onready var weapon_name_label: Label = %WeaponNameLabel
@onready var ammo_label: Label = %AmmoLabel

var current_ammo_text : int
var reserve_ammo_text : int
var current_weapon_text : StringName
var frames_per_second : StringName
var player_state_name : StringName
var weapon_state_name : StringName

func _process(_delta: float):
	current_weapon_text = weapon_manager.get_current_weapon_name()
	current_ammo_text = weapon_manager.get_current_weapon_ammo()
	reserve_ammo_text = weapon_manager.get_current_reserve_ammo()
	
	frames_per_second = str(Engine.get_frames_per_second())
	player_state_name = movementStateMachine.get_current_state_name()
	weapon_state_name = weaponStateMachine.get_current_state_name()
	
	weapon_name_label.set_text("Weapon: " + current_weapon_text)
	ammo_label.set_text("Ammo: " + 
		str(current_ammo_text) + "/" + str(reserve_ammo_text))
	fps_label.set_text("FPS: " + str(frames_per_second))
	player_state_name_label.set_text("Player State: " + player_state_name)
	weapon_state_name_label.set_text("Weapon State: " + weapon_state_name)
