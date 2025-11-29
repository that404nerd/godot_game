#include "weapon_manager.h"

WeaponManager::WeaponManager() : m_WeaponAnimPlayer(nullptr), m_CurrentWeapon(nullptr), m_WeaponSocket(nullptr), m_WeaponNode(nullptr),
                                m_MouseMovement(Vector2()),
                                m_RandSwayX(0.0f), m_RandSwayY(0.0f), m_RandSwayAmt(0.0f), m_IdleSwayRotStr(0.0f), m_IdleSwayAdj(0.0f), m_Time(0.0f)
{

}

void WeaponManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_weapon_list", "weaponList"), &WeaponManager::set_weapon_list);
    ClassDB::bind_method(D_METHOD("get_weapon_list"), &WeaponManager::get_weapon_list);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "weapon_list"), "set_weapon_list", "get_weapon_list");
    GD_BIND_PROPERTY(WeaponManager, sway_speed, Variant::FLOAT);
}

Array WeaponManager::get_weapon_list() { return m_WeaponList; }
void WeaponManager::set_weapon_list(const Array& weaponList) { m_WeaponList = weaponList; }

void WeaponManager::_ready()
{
    m_WeaponSocket = get_node<Node3D>(NodePath("../CameraController/PlayerHead/Camera3D/WeaponSocket"));

    init();
}

void WeaponManager::_input(const Ref<InputEvent>& event)
{
    Ref<InputEventMouseMotion> mouse_event = event;
    if(event->is_class("InputEventMouseMotion")) {
        m_MouseMovement = mouse_event->get_relative();
    }
}

void WeaponManager::init()
{
    m_CurrentWeapon = m_WeaponList[0];
    
    // The weapon socket is already positioned manually in the editor.
    m_WeaponNode = Object::cast_to<Node3D>(m_CurrentWeapon->get_weapon_model()->instantiate());
    m_WeaponSocket->add_child(m_WeaponNode);
    m_WeaponAnimPlayer = m_WeaponNode->get_node<AnimationPlayer>(NodePath("WeaponAnimPlayer"));
    
    if(m_WeaponAnimPlayer) {
        m_WeaponAnimPlayer->play(m_CurrentWeapon->get_weaponEquipAnimName());
    } 
}

void WeaponManager::_physics_process(double delta)
{
    if(m_CurrentWeapon.is_valid()) {
        m_Position = m_CurrentWeapon->get_position();
        m_Rotation = m_CurrentWeapon->get_rotation();
        m_Scale = m_CurrentWeapon->get_scale();
        m_RandSwayAmt = m_CurrentWeapon->get_randomSwayAmt();
        m_IdleSwayAdj = m_CurrentWeapon->get_idleSwayAdjustment();
        m_IdleSwayRotStr = m_CurrentWeapon->get_idleSwayRotationStrength();
    }

    _weapon_sway(delta);
}

void WeaponManager::_weapon_sway(double delta)
{
    // Some math for sway during the idle state
    float get_sway_amt = get_sway_speed(); // Sway speed is set in the editor
    float final_sway_amt = get_sway_amt * m_IdleSwayAdj; // Adjust the final sway amount by a idle sway adjustement multiplier

    m_Time += delta * (get_sway_speed() + final_sway_amt);
    m_RandSwayX = sin(m_Time * 1.5f + final_sway_amt) / m_RandSwayAmt; // Make this faster so that it feels natural i guess
    m_RandSwayY = sin(m_Time - final_sway_amt) / m_RandSwayAmt;

    m_MouseMovement = m_MouseMovement.clamp(m_CurrentWeapon->get_swayMin(), m_CurrentWeapon->get_swayMax());

    Vector3 weaponPos = m_WeaponNode->get_position();
    Vector3 weaponRot = m_WeaponNode->get_rotation();
    
    ///////////// Set weapon position //////////////
    weaponPos.x = Math::lerp(weaponPos.x, 
                            m_Position.x - (m_MouseMovement.x * m_CurrentWeapon->get_swayWeaponPosMult() + m_RandSwayX) * (float)delta, 
                            m_CurrentWeapon->get_swayWeaponPosLerp());
    weaponPos.y = Math::lerp(weaponPos.y, 
                            m_Position.y + (m_MouseMovement.y * m_CurrentWeapon->get_swayWeaponPosMult() + m_RandSwayY) * (float)delta, 
                            m_CurrentWeapon->get_swayWeaponPosLerp());
        
    ///////////// Set weapon rotation ////////////////
    weaponRot.x = Math::lerp(weaponRot.x, 
                            m_Rotation.x - ( m_MouseMovement.y * Math::deg_to_rad(m_CurrentWeapon->get_swayWeaponRotMult()) + (m_RandSwayX * Math::deg_to_rad(m_IdleSwayRotStr)) ) * (float)delta, 
                            m_CurrentWeapon->get_swayWeaponRotLerp());

    weaponRot.y = Math::lerp(weaponRot.y, 
                            m_Rotation.y + ( m_MouseMovement.x * Math::deg_to_rad(m_CurrentWeapon->get_swayWeaponRotMult()) + (m_RandSwayY * Math::deg_to_rad(m_IdleSwayRotStr)) ) * (float)delta, 
                            m_CurrentWeapon->get_swayWeaponRotLerp());
    
    m_WeaponNode->set_position(weaponPos);
    m_WeaponNode->set_rotation(weaponRot);

}

WeaponManager::~WeaponManager()
{
}