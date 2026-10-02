#include "anim_list_data.h"

void AnimListData::_bind_methods()
{

}

void AnimListData::do_something()
{
  if(m_AnimPlayer)
  {
    print_line(m_AnimPlayer->get_animation_list());
    m_AnimNames = m_AnimPlayer->get_animation_list(); 
  }
}

void AnimListData::_get_property_list(List<PropertyInfo> *p_list)
{
  Utils::add_property_cond(p_list, { 
    .PropertyName = "Animations", 
    .VariantType = Variant::INT, 
    .PropHint = PROPERTY_HINT_ENUM, 
    .EnumValues = StringName(",").join(m_AnimNames),
  });
}

bool AnimListData::_set(const StringName &p_name, const Variant &p_value) 
{
  if(Utils::set_property(p_name, StringName("Animations"), p_value, selected_anim))
  {
    notify_property_list_changed();
    return true;
  }


	return false;
}

bool AnimListData::_get(const StringName &p_name, Variant &r_ret) 
{
  if(Utils::get_property(p_name, "Animations", r_ret, selected_anim))
    return true;

  return false;
}