extends PanelContainer

@onready var playerStateMachine : PlayerStateMachine = %PlayerStateMachine
@onready var weaponStateMachine : WeaponStateMachine = %WeaponStateMachine
@onready var ammoComponent : AmmoComponent = %AmmoComponent
@onready var weaponComponent : WeaponComponent = %WeaponComponent

@onready var propertyContainer : VBoxContainer = %PropertyContainer

var weaponData : Weapon

var fps_label : Label = Label.new()
var player_state_label : Label = Label.new()
var weapon_state_label : Label = Label.new()
var current_weapon_ammo_label : Label = Label.new()
var current_weapon_total_ammo_label : Label = Label.new()
	
var frames_per_second : String
var player_state_name : StringName
var weapon_state_name : StringName
var current_weapon_ammo_count : StringName
var current_weapon_total_ammo_count : StringName

func _ready() -> void:
	visible = true
	add_debug_property("FPS", frames_per_second, fps_label)
	add_debug_property("Player State", player_state_name, player_state_label)
	add_debug_property("Weapon State", weapon_state_name, weapon_state_label)
	add_debug_property("Current Weapon Total Ammo", 
		current_weapon_total_ammo_count, current_weapon_total_ammo_label)
	add_debug_property("Current Weapon Ammo", current_weapon_ammo_count, 
						current_weapon_ammo_label)

func _unhandled_input(_event: InputEvent) -> void:
	if Input.is_action_just_pressed("toggle_debug_panel"):
		visible = !visible
	
func _process(_delta: float) -> void:
	
	weaponData = weaponComponent.get_current_weapon_data()
	
	if visible == true:
		frames_per_second = str(Engine.get_frames_per_second())
		player_state_name = playerStateMachine.get_current_state_name()
		weapon_state_name = weaponStateMachine.get_current_state_name()
		current_weapon_ammo_count = str(ammoComponent.get_current_weapon_ammo())
		current_weapon_total_ammo_count = str(weaponData.get_totalAmmoCount())
		
		update_debug_property(frames_per_second, fps_label)
		update_debug_property(player_state_name, player_state_label)
		update_debug_property(weapon_state_name, weapon_state_label)
		update_debug_property(current_weapon_ammo_count, current_weapon_ammo_label)
		update_debug_property(current_weapon_total_ammo_count, current_weapon_total_ammo_label)

func add_debug_property(title: String, value, property: Label):
	propertyContainer.add_child(property)
	property.name = title
	property.text = property.name + value
	
func update_debug_property(value, property: Label):
	property.text = property.name + ": " + value
