extends Control

@onready var player: Player = $"../Player"

@onready var weapon_manager: WeaponManager = player.get_weapon_manager()
@onready var movementStateMachine: MovementStateMachine = player.get_movement_state_machine()
@onready var weaponStateMachine : WeaponStateMachine = player.get_weapon_state_machine()
@onready var weaponComponent : WeaponComponent = player.get_weapon_component()

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

func _ready():
	print_rich("[color=GREEN]HUD Initialized")

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
