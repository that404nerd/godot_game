#include <godot_cpp/godot.hpp>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/look_at_modifier3d.hpp>

#include <godot_cpp/classes/scene_tree.hpp>

#include "../../globals.h"

class Player;

using namespace godot;

class LookAtPlayerComponent : public Node
{
  GDCLASS(LookAtPlayerComponent, Node);
public:
  void _init(Player* playerInst);

  void _update(double delta);
  void set_look_status(bool status) { lookAtModiferInst->set_active(status); }

protected:
  static void _bind_methods();
private:
  GD_DEFINE_PROPERTY(LookAtModifier3D*, lookAtModiferInst, nullptr);
  Player* m_PlayerInst { nullptr };
};