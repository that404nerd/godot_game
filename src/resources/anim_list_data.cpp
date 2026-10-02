#include "anim_list_data.h"

void AnimListData::_bind_methods()
{
  GD_BIND_PROPERTY(AnimListData, playback_path, Variant::STRING_NAME);
  GD_BIND_PROPERTY(AnimListData, action_name, Variant::STRING_NAME);
  GD_BIND_PROPERTY(AnimListData, underlying_anim_name, Variant::STRING_NAME);
  GD_BIND_PROPERTY(AnimListData, has_valid_anim, Variant::BOOL);
}