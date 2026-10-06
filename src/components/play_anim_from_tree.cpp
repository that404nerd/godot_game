#include "play_anim_from_tree.h"

void PlayAnimFromTree::_ready()
{
  PlayAnimComponent::_ready();

  if(Engine::get_singleton()->is_editor_hint() && !is_init)
  {
    Ref<AnimListData> animListData;
    animListData.instantiate();

    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_EQUIP).c_str(), animListData->duplicate());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_SHOOT).c_str(), animListData->duplicate());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_START).c_str(), animListData->duplicate());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD).c_str(), animListData->duplicate());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_RELOAD_END).c_str(), animListData->duplicate());
    anim_list.set(m_AnimsMap.at(AnimTypes::WEAPON_UNEQUIP).c_str(), animListData->duplicate());
  }
  is_init = true;

  if(anim_tree)
  {
    anim_tree->connect("animation_started", Callable(this, "_on_anim_started"));
    anim_tree->connect("animation_finished", Callable(this, "_on_anim_finished"));
  }

}

void PlayAnimFromTree::_on_anim_started(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_started", animName); 
}

void PlayAnimFromTree::_on_anim_finished(StringName animName)
{
  EventBus::get_singleton()->emit_signal("anim_finished", animName); 
}

bool PlayAnimFromTree::has_valid_anim(AnimTypes animType)
{
  Ref<AnimListData> data = anim_list.get(m_AnimsMap.at(animType).c_str(), nullptr);
  return data->get_has_valid_anim();
}

StringName PlayAnimFromTree::get_anim_name(AnimTypes animType)
{
  Ref<AnimListData> data = anim_list.get(m_AnimsMap.at(animType).c_str(), nullptr);

  if(!data.is_valid())
    return "";

  return data->get_underlying_anim_name();
}

void PlayAnimFromTree::execute_anim(AnimTypes animType)
{
  Ref<AnimListData> data = anim_list.get(m_AnimsMap.at(animType).c_str(), nullptr);

  if(!data.is_valid())
    return;

  StringName playback_path = data->get_playback_path();
  m_AnimTreePlayback = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get(playback_path));

  StringName action_name = data->get_action_name();
  StringName travel_path = playback_path.path_join(action_name);

  m_AnimTreePlayback->travel(action_name);
  m_AnimTreePlayback->start(action_name);
}

void PlayAnimFromTree::_bind_methods()
{
  GD_BIND_CUSTOM_PROPERTY(PlayAnimFromTree, AnimationTree, anim_tree, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE);
  GD_BIND_PROPERTY(PlayAnimFromTree, is_init, Variant::BOOL);
  GD_BIND_PROPERTY(PlayAnimFromTree, anim_list, Variant::DICTIONARY);
}